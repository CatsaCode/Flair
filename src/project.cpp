#include "project.hpp"
#include "main.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/TextureFormat.hpp"

#include "UnityEngine/Resources.hpp" // DEBUG

using namespace Flair;
using namespace UnityEngine;

Project::Project() {

}

Project::Project(std::string_view filePath) {
    LoadFromFile(filePath);
}

void Project::LoadFromFile(std::string_view filePath) {
    PaperLogger.info("Loading project from file");

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(std::string(filePath), 
        aiProcess_Triangulate
    );

    if(scene == nullptr) {
        PaperLogger.error("{}", importer.GetErrorString());
        return;
    }

    LoadMeshes(scene);
    LoadTextures(scene);
    LoadMaterials(scene);
}

void Project::LoadMeshes(const aiScene* scene) {
    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        Mesh* unityMesh = Mesh::New_ctor();
        unityMesh->set_name(mesh->mName.C_Str());
        unityMesh->Clear();

        if(mesh->HasPositions()) {
            ArrayW<Vector3> vertices (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D& vertex = mesh->mVertices[j];
                vertices[j] = Vector3(vertex.x, vertex.y, vertex.z);
            }
            unityMesh->set_vertices(vertices);
        }

        if(mesh->HasFaces()) {
            ArrayW<int> triangles (mesh->mNumFaces * 3);
            for(int j = 0; j < mesh->mNumFaces; j++) {
                aiFace& face = mesh->mFaces[j];
                for(int k = 0; k < 3; k++) {
                    triangles[j * 3 + k] = face.mIndices[k];
                }
            }
            unityMesh->set_triangles(triangles);
        }

        if(mesh->HasNormals()) {
            ArrayW<Vector3> normals (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D& normal = mesh->mNormals[j];
                normals[j] = Vector3(normal.x, normal.y, normal.z);
            }
            unityMesh->set_normals(normals);
        } else {
            unityMesh->RecalculateNormals();
        }

        // TODO Import tangent data from assimp
        unityMesh->RecalculateTangents();

        if(mesh->HasVertexColors(0)) {
            ArrayW<Color> colors (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiColor4D& color = mesh->mColors[i][j];
                colors[j] = Color(color.r, color.g, color.b, color.a);
            }
            unityMesh->set_colors(colors);
        }

        for(int j = 0; j < 8; j++) {
            if(!mesh->HasTextureCoords(j)) continue;
            ArrayW<Vector2> uvs (mesh->mNumVertices);
            for(int k = 0; k < mesh->mNumVertices; k++) {
                aiVector3D& uv = mesh->mTextureCoords[j][k];
                uvs[k] = Vector2(uv.x, uv.y);
            }
            unityMesh->SetUVs(j, uvs);
        }

        meshes.push_back(unityMesh);
    }
}

void Project::LoadTextures(const aiScene* scene) {
    for(int i = 0; i < scene->mNumTextures; i++) {
        aiTexture* texture = scene->mTextures[i];

        if(texture->mHeight > 0) {
            PaperLogger.error("Texture {} is uncompressed!", i); // TODO
            continue;
        }

        ArrayW<uint8_t> imageData (texture->mWidth * 4);
        for(int j = 0; j < texture->mWidth; j++) {
            aiTexel& texel = texture->pcData[j]; // Assimp is ARGB, Unity is BGRA
            imageData[j * 4 + 0] = texel.b;
            imageData[j * 4 + 1] = texel.g;
            imageData[j * 4 + 2] = texel.r;
            imageData[j * 4 + 3] = texel.a;
        }

        Texture2D* unityTexture = Texture2D::New_ctor(0, 0, TextureFormat::RGBA32, false, false); // Size is updated automatically
        if(!ImageConversion::LoadImage(unityTexture, imageData)) {
            PaperLogger.error("Failed to load texture {}!", i);
            continue;
        }

        textures.push_back(unityTexture);
    }
}

void Project::LoadMaterials(const aiScene* scene) {
    // DEBUG
    Shader* shader = Resources::FindObjectsOfTypeAll<Shader*>()->FirstOrDefault([](Shader* shader){return shader->get_name() == "Standard";});
    Material* material = Material::New_ctor(shader);

    if(textures.size() > 0) material->set_mainTexture(textures[0].ptr());

    materials.push_back(material);
}