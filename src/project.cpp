#include "project.hpp"
#include "main.hpp"
#include "loggingUtils.hpp"

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
#include "UnityEngine/MeshFilter.hpp" // DEBUG
#include "UnityEngine/MeshRenderer.hpp" // DEBUG

using namespace Flair;
using namespace UnityEngine;

// DEBUG
void LogHierarchy(const aiNode* node, const int depth = 0) {
    std::string depthStr = "";
    for(int i = 0; i < depth; i++) depthStr += DEPTH_STR;

    aiVector3D scale;
    aiVector3D rotation;
    aiVector3D position;
    node->mTransformation.Decompose(scale, rotation, position);

    std::vector<int> meshIndices;
    for(int i = 0; i < node->mNumMeshes; i++) meshIndices.push_back(node->mMeshes[i]);

    PaperLogger.info("{}", depthStr);
    PaperLogger.info("{}Name: {}", depthStr, node->mName.C_Str());
    PaperLogger.info("{}- Position: ({:.1f}, {:.1f}, {:.1f})", depthStr, position.x, position.y, position.z);
    PaperLogger.info("{}- Rotation: ({:.1f}, {:.1f}, {:.1f})", depthStr, rotation.x, rotation.y, rotation.z);
    PaperLogger.info("{}- Scale: ({:.1f}, {:.1f}, {:.1f})", depthStr, scale.x, scale.y, scale.z);
    PaperLogger.info("{}- Meshes: {}", depthStr, meshIndices);

    if(node->mMetaData != nullptr) {
        for(int i = 0; i < node->mMetaData->mNumProperties; i++) {
            std::string_view key = node->mMetaData->mKeys[i].C_Str();
            aiMetadataType type = node->mMetaData->mValues[i].mType;
            // TODO: Convert void* to type
            PaperLogger.info("{}- MD {}", depthStr, i);
        }
    }

    for(int i = 0; i < node->mNumChildren; i++) {
        aiNode* child = node->mChildren[i];
        LogHierarchy(child, depth + 1);
    }
}

Project::Project() {

}

Project::Project(std::string_view filePath) {
    LoadFromFile(filePath);
}

void Project::LoadFromFile(std::string_view filePath) {
    PaperLogger.info("Loading project from {}", filePath);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(std::string(filePath), 
        aiProcess_Triangulate |
        aiProcess_MakeLeftHanded
    );

    if(scene == nullptr) {
        PaperLogger.error("{}", importer.GetErrorString());
        return;
    }

    LogHierarchy(scene->mRootNode);

    LoadMeshes(scene);
    LoadTextures(scene);
    LoadMaterials(scene);
    LoadPrefabs(scene);
}

void Project::LoadMeshes(const aiScene* scene) {
    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];

        Mesh* unityMesh = Mesh::New_ctor();
        if(LOG_MESH_DATA) PaperLogger.info("Scene mesh #: {}, Name: {}", i, mesh->mName.C_Str());
        unityMesh->set_name(mesh->mName.C_Str());
        unityMesh->Clear();

        if(mesh->HasPositions()) {
            ArrayW<Vector3> vertices (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D& vertex = mesh->mVertices[j];
                if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Vertex #: {}, Position: ({:.2f}, {:.2f}, {:.2f})", j, vertex.x, vertex.y, vertex.z);
                vertices[j] = Vector3(vertex.x, vertex.y, vertex.z);
            }
            unityMesh->set_vertices(vertices);
        }

        if(mesh->HasFaces()) {
            ArrayW<int> triangles (mesh->mNumFaces * 3);
            for(int j = 0; j < mesh->mNumFaces; j++) {
                aiFace& face = mesh->mFaces[j];
                if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Face #: {}, # Indices: {}, Indices: [{}, {}, {}]", j, face.mNumIndices, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
                for(int k = 0; k < 3; k++) {
                    triangles[j * 3 + k] = face.mIndices[2 - k];
                }
            }
            unityMesh->set_triangles(triangles);
        }

        if(mesh->HasNormals()) {
            ArrayW<Vector3> normals (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D& normal = mesh->mNormals[j];
                if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Normal #: {}, Normal: ({:.2f}, {:.2f}, {:.2f})", j, normal.x, normal.y, normal.z);
                normals[j] = Vector3(normal.x, normal.y, normal.z);
            }
            unityMesh->set_normals(normals);
        } else {
            if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Recalculating normals...");
            unityMesh->RecalculateNormals();
        }

        // TODO Import tangent data from assimp
        if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Recalculating tangents...");
        unityMesh->RecalculateTangents();

        if(mesh->HasVertexColors(0)) {
            ArrayW<Color> colors (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiColor4D& color = mesh->mColors[i][j];
                if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Color #: {}, Color: ({:.2f}, {:.2f}, {:.2f}, {:.2f})", j, color.r, color.g, color.b, color.a);
                colors[j] = Color(color.r, color.g, color.b, color.a);
            }
            unityMesh->set_colors(colors);
        }

        for(int j = 0; j < 8; j++) {
            if(!mesh->HasTextureCoords(j)) continue;
            ArrayW<Vector2> uvs (mesh->mNumVertices);
            for(int k = 0; k < mesh->mNumVertices; k++) {
                aiVector3D& uv = mesh->mTextureCoords[j][k];
                if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "UV #: {}, Vertex #: {}, UV: ({:.2f}, {:.2f}, {:.2f})", j, k, uv.x, uv.y, uv.z);
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
        if(LOG_TEXTURE_DATA) PaperLogger.info("Scene texture #: {}, File name: {}, Format hint: {}, Width: {}, Height: {}", i, texture->mFilename.C_Str(), texture->achFormatHint, texture->mWidth, texture->mHeight);

        if(texture->mHeight > 0) {
            PaperLogger.error("Texture {} is uncompressed!", i); // TODO
            continue;
        }

        ArrayW<uint8_t> imageData (texture->mWidth * 4);
        for(int j = 0; j < texture->mWidth; j++) {
            aiTexel& texel = texture->pcData[j]; 
            // Assimp is ARGB, Unity is BGRA
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

void Project::LoadPrefabs(const aiScene* scene) {
    GameObject* prefab = GameObject::New_ctor("TestPrefab");
    Object::DontDestroyOnLoad(prefab);
    prefab->SetActive(false);
    prefabs.push_back(prefab);

    MeshFilter* testFilter = prefab->AddComponent<MeshFilter*>();
    testFilter->set_sharedMesh(meshes[0].ptr());
    
    MeshRenderer* testRenderer = prefab->AddComponent<MeshRenderer*>();
    testRenderer->set_sharedMaterial(materials[0].ptr());
}