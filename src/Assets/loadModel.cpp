#include "assets.hpp"
#include "main.hpp"
#include "logging.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/material.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/CombineInstance.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Rendering/LocalKeyword.hpp"

#include <cstring>
#include <string_view>

using namespace UnityEngine;
using namespace UnityEngine::Rendering;

namespace Flair::Assets {

    GameObject* loadModel(std::string_view filePath) {
        PaperLogger.info("Loading model from {}", filePath);

        if(LOG_LOAD_MODEL) PaperLogger.info("Loading scene...");
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(std::string(filePath), 
            aiProcess_Triangulate |
            aiProcess_MakeLeftHanded
        );

        if(scene == nullptr) {PaperLogger.error("{}", importer.GetErrorString()); return nullptr;}
        if(LOG_LOAD_MODEL_SCENE) logHierarchy(scene->mRootNode);
        if(!scene->mRootNode) {PaperLogger.warn("Scene is missing a root node"); return nullptr;}

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting meshes...");
        std::vector<Mesh*> unityMeshes = {};
        std::vector<std::vector<int>> submeshIndices = {}; // Meshes with multiple materials are split into submeshes, this keeps tracks of which assimp (sub)meshes each unityMesh references
        std::vector<int> meshMaterialIndices = {}; // Which material each unityMaterial references
        for(int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];
            Mesh* unityMesh = assimpToUnity(mesh);
            if(!unityMesh) {PaperLogger.warn("Failed to convert mesh #: {}", i); return nullptr;}
            unityMeshes.push_back(unityMesh);
            submeshIndices.push_back({i});
            meshMaterialIndices.push_back(mesh->mMaterialIndex);
        }

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting textures...");
        std::vector<Texture2D*> unityTextures = {};
        for(int i = 0; i < scene->mNumTextures; i++) {
            aiTexture* texture = scene->mTextures[i];
            Texture2D* unityTexture = assimpToUnity(texture);
            if(!unityTexture) {PaperLogger.warn("Failed to convert texture #: {}", i); return nullptr;}
            unityTextures.push_back(unityTexture);
        }

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting materials...");
        std::vector<Material*> unityMaterials = {};
        for(int i = 0; i < scene->mNumMaterials; i++) {
            aiMaterial* material = scene->mMaterials[i];
            Material* unityMaterial = assimpToUnity(material);
            if(!unityMaterial) {PaperLogger.warn("Failed to convert material #: {}", i); return nullptr;}
            unityMaterials.push_back(unityMaterial);
        }

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting nodes...");
        GameObject* unityGO = assimpToUnity(scene->mRootNode, unityMeshes, submeshIndices, unityMaterials, meshMaterialIndices);
        return unityGO;
    }

    Mesh* assimpToUnity(const aiMesh* mesh) {
        Mesh* unityMesh = Mesh::New_ctor();
        if(LOG_A2U_MESH_DATA) PaperLogger.info("Mesh name: {}", mesh->mName.C_Str());
        unityMesh->set_name(mesh->mName.C_Str());
        unityMesh->Clear();

        if(mesh->HasPositions()) {
            ArrayW<Vector3> vertices (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D& vertex = mesh->mVertices[j];
                if(LOG_A2U_MESH_DATA) PaperLogger.info("Vertex #: {}, Position: ({:.2f}, {:.2f}, {:.2f})", j, vertex.x, vertex.y, vertex.z);
                vertices[j] = Vector3(vertex.x, vertex.y, vertex.z);
            }
            unityMesh->set_vertices(vertices);
        }

        if(mesh->HasFaces()) {
            ArrayW<int> triangles (mesh->mNumFaces * 3);
            for(int j = 0; j < mesh->mNumFaces; j++) {
                aiFace& face = mesh->mFaces[j];
                if(LOG_A2U_MESH_DATA) PaperLogger.info("Face #: {}, # Indices: {}, Indices: [{}, {}, {}]", j, face.mNumIndices, face.mIndices[0], face.mIndices[1], face.mIndices[2]);
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
                if(LOG_A2U_MESH_DATA) PaperLogger.info("Normal #: {}, Normal: ({:.2f}, {:.2f}, {:.2f})", j, normal.x, normal.y, normal.z);
                normals[j] = Vector3(normal.x, normal.y, normal.z);
            }
            unityMesh->set_normals(normals);
        } else {
            if(LOG_A2U_MESH_DATA) PaperLogger.info("Recalculating normals...");
            unityMesh->RecalculateNormals();
        }

        // TODO Import tangent data from assimp
        if(LOG_A2U_MESH_DATA) PaperLogger.info("Recalculating tangents...");
        unityMesh->RecalculateTangents();

        if(mesh->HasVertexColors(0)) {
            ArrayW<Color> colors (mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiColor4D& color = mesh->mColors[0][j];
                if(LOG_A2U_MESH_DATA) PaperLogger.info("Color #: {}, Color: ({:.2f}, {:.2f}, {:.2f}, {:.2f})", j, color.r, color.g, color.b, color.a);
                colors[j] = Color(color.r, color.g, color.b, color.a);
            }
            unityMesh->set_colors(colors);
        }

        for(int j = 0; j < 8; j++) {
            if(!mesh->HasTextureCoords(j)) continue;
            ArrayW<Vector2> uvs (mesh->mNumVertices);
            for(int k = 0; k < mesh->mNumVertices; k++) {
                aiVector3D& uv = mesh->mTextureCoords[j][k];
                if(LOG_A2U_MESH_DATA) PaperLogger.info("UV #: {}, Vertex #: {}, UV: ({:.2f}, {:.2f}, {:.2f})", j, k, uv.x, uv.y, uv.z);
                uvs[k] = Vector2(uv.x, uv.y);
            }
            unityMesh->SetUVs(j, uvs);
        }

        if(LOG_A2U_MESH_DATA) PaperLogger.info("Material #: {}", mesh->mMaterialIndex);

        return unityMesh;
    }

    Texture2D* assimpToUnity(const aiTexture* texture) {
        if(LOG_A2U_TEXTURE_INFO) PaperLogger.info("Texture: '{}', Format hint: '{}', Width: {}, Height: {}", texture->mFilename.C_Str(), texture->achFormatHint, texture->mWidth, texture->mHeight);

        if(texture->mHeight > 0) {
            PaperLogger.error("Can't load uncompressed texture '{}'", texture->mFilename.C_Str()); // TODO
            return nullptr;
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
            PaperLogger.error("Failed to load texture '{}'", texture->mFilename.C_Str());
            return nullptr;
        }

        return unityTexture;
    }

    Material* assimpToUnity(const aiMaterial* material) {
        if(LOG_A2U_MATERIAL_DATA) {
            PaperLogger.info("Material: '{}'", material->GetName().C_Str());
            PaperLogger.info("# Properties: {}", material->mNumProperties);
            const char* typeNames[] = {"___", "float", "double", "string", "integer", "buffer"};
            for(int i = 0; i < material->mNumProperties; i++) {
                aiMaterialProperty* matProp = material->mProperties[i];
                PaperLogger.info("Material property: {}, Type: '{}', Size: {}, Texture semantic: {}, Texture #: {}", matProp->mKey.C_Str(), typeNames[matProp->mType], matProp->mDataLength, matProp->mSemantic, matProp->mIndex);
            }
        }

        // DEBUG
        PaperLogger.info("Creating debug material...");
        static SafePtrUnity<Shader> shader;
        if(!shader) shader = Shader::Find("Standard");
        
        if(LOG_A2U_MATERIAL_DATA) {
            PaperLogger.info("Shader: '{}'", shader->get_name());
            PaperLogger.info("# Properties: {}", shader->GetPropertyCount());
            const char* typeNames[] = {"Color", "Vector", "Float", "Range/Float", "Texture", "Int"};
            for(int i = 0; i < shader->GetPropertyCount(); i++) {
                PaperLogger.info("Shader property: '{}', Type: '{}', Description: '{}'", shader->GetPropertyName(i), typeNames[(int)shader->GetPropertyType(i)], shader->GetPropertyDescription(i));
            }
        }
        
        Material* unityMaterial = Material::New_ctor(shader.ptr());
        unityMaterial->set_name(material->GetName().C_Str());

        aiColor3D colorDiffuse;
        if(material->Get(AI_MATKEY_COLOR_DIFFUSE, colorDiffuse) == AI_SUCCESS) {
            if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$clr.diffuse' to '_Color': ({:.2f}, {:.2f}, {:.2f}, [1])", colorDiffuse.r, colorDiffuse.g, colorDiffuse.b);
            unityMaterial->SetColor("_Color", Color(colorDiffuse.r, colorDiffuse.g, colorDiffuse.b, 1));
        }

        aiColor3D baseColor;
        if(material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
            if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$clr.base' to '_Color': ({:.2f}, {:.2f}, {:.2f}, [1])", baseColor.r, baseColor.g, baseColor.b);
            unityMaterial->SetColor("_Color", Color(baseColor.r, baseColor.g, baseColor.b, 1));
        }

        // Zero metallic on the Standard shader just makes the material black
        // float metallicFactor;
        // if(material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor) == AI_SUCCESS) {
        //     if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$mat.metallicFactor' to '_Metallic': {:.2f}", metallicFactor);
        //     unityMaterial->SetFloat("_Metallic", metallicFactor);
        // }
        if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Forcing '_Metallic' to 1");
        unityMaterial->SetFloat("_Metallic", 1);

        float roughnessFactor;
        if(material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor) == AI_SUCCESS) {
            if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$mat.roughnessFactor' to '_Glossiness': {:.2f} --> {:.2f}", roughnessFactor, 1 - roughnessFactor);
            unityMaterial->SetFloat("_Glossiness", 1 - roughnessFactor);
        }

        aiColor3D emissive;
        float emissiveIntensity;
        if(material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS &&
            material->Get(AI_MATKEY_EMISSIVE_INTENSITY, emissiveIntensity) == AI_SUCCESS
        ) {
            // Beat Saber does not have a shader varient of Standard compiled with _EMISSION
            // if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$clr.emissive' and '$mat.emissiveIntensity' to '_EmissionColor': (({:.2f}, {:.2f}, {:.2f}) * {:.2f}, [1])", emissive.r, emissive.g, emissive.b, emissiveIntensity);
            // unityMaterial->EnableKeyword("_EMISSION");
            // unityMaterial->SetColor("_EmissionColor", Color(emissive.r * emissiveIntensity, emissive.g * emissiveIntensity, emissive.b * emissiveIntensity, 1));
            if(LOG_A2U_MATERIAL_DATA) PaperLogger.info("Converting '$clr.emissive' to '_Color': ({:.2f}, {:.2f}, {:.2f}, [1])", emissive.r, emissive.g, emissive.b);
            unityMaterial->SetColor("_Color", Color(emissive.r, emissive.g, emissive.b, 1));
        }

        // if(textures.size() > 0) material->set_mainTexture(textures[0].second.ptr());

        // Next task reminder: Modify testHierarchy2 to include a second material with a second texture

        return unityMaterial;
    }

    void setupMeshFilter(GameObject* unityGO, const aiNode* node, std::vector<Mesh*>& unityMeshes, std::vector<std::vector<int>>& submeshIndices) {
        std::vector<int> targetSubmeshIndices = {};
        for(int i = 0; i < node->mNumMeshes; i++) targetSubmeshIndices.push_back(node->mMeshes[i]); // TODO Is this always in ascending order?
        int unityMeshIndex = std::distance(submeshIndices.begin(), std::find(submeshIndices.begin(), submeshIndices.end(), targetSubmeshIndices));
        if(unityMeshIndex == submeshIndices.size()) unityMeshIndex = -1;
        if(LOG_A2U_NODE_DATA) PaperLogger.info("Target submesh indices: {}, Found Unity mesh #: {}", targetSubmeshIndices, unityMeshIndex);

        if(unityMeshIndex < 0) {
            ArrayW<CombineInstance> combine = ArrayW<CombineInstance>(node->mNumMeshes);
            for(int i = 0; i < node->mNumMeshes; i++) {
                if(LOG_A2U_NODE_DATA) PaperLogger.info("Combining mesh #: {}, Mesh: '{}'", i, unityMeshes[i]->get_name());
                combine[i].set_mesh(unityMeshes[node->mMeshes[i]]);
                combine[i].set_transform(Matrix4x4::get_identity());
            }

            std::string meshName = unityMeshes[node->mMeshes[0]]->get_name();
            int hyphenIndex = meshName.find_last_not_of("0123456789");
            if(hyphenIndex < 0 || meshName[hyphenIndex] != '-') {
                PaperLogger.warn("Could not generate a combined mesh name from '{}'", meshName);
                meshName = "Unnamed mesh";
            } else {
                meshName = meshName.substr(0, hyphenIndex);
            }

            Mesh* combinedMesh = Mesh::New_ctor();
            combinedMesh->set_name(meshName);
            combinedMesh->CombineMeshes(combine, false);

            if(LOG_A2U_NODE_DATA) PaperLogger.info("Pushing as mesh #: {}", unityMeshes.size());
            unityMeshes.push_back(combinedMesh);
            submeshIndices.push_back(targetSubmeshIndices);
            unityMeshIndex = unityMeshes.size() - 1;
        }

        Mesh* unityMesh = unityMeshes[unityMeshIndex];
        if(LOG_A2U_NODE_DATA) PaperLogger.info("Mesh: '{}'", unityMesh->get_name());

        MeshFilter* unityFilter = unityGO->AddComponent<MeshFilter*>();
        unityFilter->set_sharedMesh(unityMesh);
    }

    void setupMeshRenderer(GameObject* unityGO, const aiNode* node, const std::vector<Material*>& unityMaterials, const std::vector<int>& meshMaterialIndices) {
        ArrayW<Material*> targetMaterials (node->mNumMeshes);
        for(int i = 0; i < node->mNumMeshes; i++) {
            int meshIndex = node->mMeshes[i];
            int materialIndex = meshMaterialIndices[meshIndex];
            targetMaterials[i] = unityMaterials[materialIndex];
            if(LOG_A2U_MESH_DATA) PaperLogger.info("Material #: {}, Name: '{}'", materialIndex, targetMaterials[i]->get_name());
        }

        MeshRenderer* unityRenderer = unityGO->AddComponent<MeshRenderer*>();
        unityRenderer->set_sharedMaterials(targetMaterials);
    }

    GameObject* assimpToUnity(const aiNode* node, std::vector<Mesh*>& unityMeshes, std::vector<std::vector<int>>& submeshIndices, const std::vector<Material*>& unityMaterials, const std::vector<int>& meshMaterialIndices) {
        if(LOG_A2U_NODE_DATA) PaperLogger.info("Node: '{}'", node->mName.C_Str());
        GameObject* unityGO = GameObject::New_ctor(node->mName.C_Str());

        Transform* unityTransform = unityGO->get_transform();
        aiVector3D nodePosition;
        aiVector3D nodeRotation;
        aiVector3D nodeScale;
        node->mTransformation.Decompose(nodeScale, nodeRotation, nodePosition);
        if(LOG_A2U_NODE_DATA) {
            PaperLogger.info("Local position: ({:.1f}, {:.1f}, {:.1f})", nodePosition.x, nodePosition.y, nodePosition.z);
            PaperLogger.info("Local rotation: ({:.1f}, {:.1f}, {:.1f})", nodeRotation.x, nodeRotation.y, nodeRotation.z);
            PaperLogger.info("Local scale: ({:.1f}, {:.1f}, {:.1f})", nodeScale.x, nodeScale.y, nodeScale.z);
        }
        unityTransform->set_localPosition(Vector3(nodePosition.x, nodePosition.y, nodePosition.z));
        unityTransform->set_localEulerAngles(Vector3(nodeRotation.x, nodeRotation.y, nodeRotation.z));
        unityTransform->set_localScale(Vector3(nodeScale.x, nodeScale.y, nodeScale.z));

        if(node->mNumMeshes > 0) {
            setupMeshFilter(unityGO, node, unityMeshes, submeshIndices);
            setupMeshRenderer(unityGO, node, unityMaterials, meshMaterialIndices);
        }

        for(int i = 0; i < node->mNumChildren; i++) {
            aiNode* child = node->mChildren[i];
            GameObject* unityChildGO = assimpToUnity(child, unityMeshes, submeshIndices, unityMaterials, meshMaterialIndices);
            unityChildGO->get_transform()->SetParent(unityTransform, false);
        }

        return unityGO;
    }

}