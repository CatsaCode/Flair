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
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/CombineInstance.hpp"
#include "UnityEngine/MeshRenderer.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <string_view>

using namespace UnityEngine;
using namespace UnityEngine::Rendering;

namespace Flair::Assets {

    // TODO Find a home for this and other file path manipulation functions
    std::string getFolderPath(std::string_view filePath) {
        return std::string(filePath.substr(0, filePath.find_last_of('/') + 1));
    }

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

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting meshes... ({})", scene->mNumMeshes);
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

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting embedded textures... ({})", scene->mNumTextures);
        std::vector<Texture2D*> unityTextures = {};
        for(int i = 0; i < scene->mNumTextures; i++) {
            aiTexture* texture = scene->mTextures[i];
            Texture2D* unityTexture = assimpToUnity(texture);
            if(!unityTexture) {PaperLogger.warn("Failed to convert texture #: {}", i); return nullptr;}
            unityTextures.push_back(unityTexture);
        }

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting materials... ({})", scene->mNumMaterials);
        std::vector<Material*> unityMaterials = {};
        for(int i = 0; i < scene->mNumMaterials; i++) {
            aiMaterial* material = scene->mMaterials[i];
            Material* unityMaterial = assimpToUnity(material, unityTextures, filePath);
            if(!unityMaterial) {PaperLogger.warn("Failed to convert material #: {}", i); return nullptr;}
            unityMaterials.push_back(unityMaterial);
        }

        if(LOG_LOAD_MODEL) PaperLogger.info("Converting nodes...");
        GameObject* unityGO = assimpToUnity(scene->mRootNode, unityMeshes, submeshIndices, unityMaterials, meshMaterialIndices);
        return unityGO;
    }

    Mesh* assimpToUnity(const aiMesh* mesh) {
        Mesh* unityMesh = Mesh::New_ctor();
        if(LOG_A2U_MESH_INFO) PaperLogger.info("Mesh: '{}'", mesh->mName.C_Str());
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
            if(LOG_A2U_TEXTURE_DATA && j % (texture->mWidth / 10) == 0) PaperLogger.info("Reading texel packet # {}...", j);
            aiTexel& texel = texture->pcData[j]; 
            // Assimp is ARGB, Unity is BGRA
            imageData[j * 4 + 0] = texel.b;
            imageData[j * 4 + 1] = texel.g;
            imageData[j * 4 + 2] = texel.r;
            imageData[j * 4 + 3] = texel.a;
        }

        Texture2D* unityTexture = Texture2D::New_ctor(0, 0, TextureFormat::RGBA32, false, false); // Size is updated automatically
        unityTexture->set_name(texture->mFilename.C_Str()); // TODO This name can be ''
        if(!ImageConversion::LoadImage(unityTexture, imageData)) {PaperLogger.error("Failed to load texture '{}'", texture->mFilename.C_Str()); return nullptr;}
        
        return unityTexture;
    }

    Texture2D* getMaterialUnityTexture(const aiMaterial* material, const aiTextureType textureType, std::vector<Texture2D*>& unityTextures, std::string_view filePath) {
        aiString texturePath;
        aiTextureMapMode textureMapMode;
        if(material->GetTexture(textureType, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, &textureMapMode) != AI_SUCCESS) return nullptr;
        if(texturePath.length <= 0) {PaperLogger.warn("Texture path is empty"); return nullptr;}

        Texture2D* unityTexture;
        if(texturePath.data[0] == '*') {
            int textureIndex = std::stoi(std::string(texturePath.data).substr(1, texturePath.length - 1));
            if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Getting texture from # {}", textureIndex);
            unityTexture = unityTextures[textureIndex];

        } else {
            auto textureIt = std::find_if(unityTextures.begin(), unityTextures.end(), [&texturePath](Texture2D* tex){return tex->get_name() == texturePath.C_Str();});
            if(textureIt != unityTextures.end()) {
                if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Getting texture from name '{}'", texturePath.C_Str());
                int textureIndex = std::distance(unityTextures.begin(), textureIt);
                unityTexture = unityTextures[textureIndex];

            } else {
                if(LOG_A2U_MATERIAL_INFO || LOG_A2U_TEXTURE_INFO) PaperLogger.info("Generating new texture '{}'", texturePath.C_Str());
                std::string fullTexturePath = getFolderPath(filePath) + texturePath.C_Str();
                std::ifstream file (fullTexturePath, std::ios::in | std::ios::binary | std::ios::ate);
                if(!file.is_open()) {PaperLogger.warn("Couldn't open texture file '{}'", fullTexturePath); return nullptr;}

                size_t fileSize = file.tellg();
                ArrayW<uint8_t> fileData (fileSize);
                file.seekg(0, file.beg);
                file.read(reinterpret_cast<char*>(fileData->_values), fileSize);
                file.close();

                unityTexture = Texture2D::New_ctor(0, 0, TextureFormat::RGBA32, false, false);
                unityTexture->set_name(texturePath.C_Str());
                if(!ImageConversion::LoadImage(unityTexture, fileData)) {PaperLogger.error("Failed to load texture file '{}'", fullTexturePath); return nullptr;}
                unityTextures.push_back(unityTexture);
            }
        }

        if(textureMapMode == aiTextureMapMode_Wrap) unityTexture->set_wrapMode(TextureWrapMode::Repeat);
        else if(textureMapMode == aiTextureMapMode_Clamp) unityTexture->set_wrapMode(TextureWrapMode::Clamp);
        else if(textureMapMode == aiTextureMapMode_Mirror) unityTexture->set_wrapMode(TextureWrapMode::Mirror);
        else if(textureMapMode == aiTextureMapMode_Decal) {unityTexture->set_wrapMode(TextureWrapMode::Repeat); PaperLogger.warn("No conversion for aiTextureMapMode_Decal");}

        return unityTexture;
    }

    Material* assimpToUnity(const aiMaterial* material, std::vector<Texture2D*>& unityTextures, std::string_view filePath) {
        if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Material: '{}'", material->GetName().C_Str());
        if(LOG_A2U_MATERIAL_DATA) {
            PaperLogger.info("# Properties: {}", material->mNumProperties);
            const char* propertyTypeNames[] = {"___", "float", "double", "string", "integer", "buffer"};
            for(int i = 0; i < material->mNumProperties; i++) {
                aiMaterialProperty* matProp = material->mProperties[i];
                PaperLogger.info("Material property: '{}', Type: '{}', Size: {}, Texture semantic: {}, Texture #: {}", matProp->mKey.C_Str(), propertyTypeNames[matProp->mType], matProp->mDataLength, matProp->mSemantic, matProp->mIndex);
            }

            const char* textureTypeNames[] = {
                "aiTextureType_NONE", "aiTextureType_DIFFUSE", "aiTextureType_SPECULAR", "aiTextureType_AMBIENT", "aiTextureType_EMISSIVE", "aiTextureType_HEIGHT", "aiTextureType_NORMALS",
                "aiTextureType_SHININESS", "aiTextureType_OPACITY", "aiTextureType_DISPLACEMENT", "aiTextureType_LIGHTMAP", "aiTextureType_REFLECTION", "aiTextureType_BASE_COLOR", "aiTextureType_NORMAL_CAMERA",
                "aiTextureType_EMISSION_COLOR", "aiTextureType_METALNESS", "aiTextureType_DIFFUSE_ROUGHNESS", "aiTextureType_AMBIENT_OCCLUSION", "aiTextureType_UNKNOWN", "aiTextureType_SHEEN", "aiTextureType_CLEARCOAT", "aiTextureType_TRANSMISSION"
            };
            for(int i = 1; i <= 21; i++) {
                PaperLogger.info("# '{}' textures: {}", textureTypeNames[i], material->GetTextureCount(static_cast<aiTextureType>(i)));
            }
        }

        // DEBUG
        if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Getting shader 'Standard'...");
        static SafePtrUnity<Shader> shader;
        if(!shader) shader = Shader::Find("Standard");

        if(LOG_A2U_MATERIAL_DATA) {
            PaperLogger.info("Shader: '{}'", shader->get_name());

            PaperLogger.info("# Properties: {}", shader->GetPropertyCount());
            const char* typeNames[] = {"Color", "Vector", "Float", "Range/Float", "Texture", "Int"};
            for(int i = 0; i < shader->GetPropertyCount(); i++) {
                PaperLogger.info("Shader property: '{}', Type: '{}', Description: '{}'", shader->GetPropertyName(i), typeNames[(int)shader->GetPropertyType(i)], shader->GetPropertyDescription(i));
            }

            static auto get_keywordNames = il2cpp_utils::resolve_icall<ArrayW<StringW>, ByRef<LocalKeywordSpace>>("UnityEngine.Rendering.LocalKeywordSpace::GetKeywordNames_Injected");
            LocalKeywordSpace localKeywordSpace = shader->get_keywordSpace();
            ArrayW<StringW> keywordNames = get_keywordNames(localKeywordSpace);
            PaperLogger.info("# Keywords: {}", keywordNames.size());
            for(int i = 0; i < keywordNames.size(); i++) {
                PaperLogger.info("Shader keyword: '{}'", keywordNames[i]);
            }
        }

        if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Creating material...");
        Material* unityMaterial = Material::New_ctor(shader.ptr());
        unityMaterial->set_name(material->GetName().C_Str());
        

        aiColor3D baseColor;
        if(material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS) {
            if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Converting '$clr.base' to '_Color': ({:.2f}, {:.2f}, {:.2f}, [1])", baseColor.r, baseColor.g, baseColor.b);
            unityMaterial->SetColor("_Color", Color(baseColor.r, baseColor.g, baseColor.b, 1));
        }

        Texture2D* baseColorUnityTexture = getMaterialUnityTexture(material, aiTextureType_BASE_COLOR, unityTextures, filePath);
        if(baseColorUnityTexture) {
            if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Converting 'aiTextureType_BASE_COLOR' # 0 to '_MainTex': (Texture: '{}')", baseColorUnityTexture->get_name());
            unityMaterial->SetTexture("_MainTex", baseColorUnityTexture);
        }

        aiColor3D emissive;
        float emissiveIntensity;
        if(material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS &&
            material->Get(AI_MATKEY_EMISSIVE_INTENSITY, emissiveIntensity) == AI_SUCCESS
        ) {
            // Beat Saber does not have a shader varient of Standard compiled with _EMISSION
            if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Converting '$clr.emissive' to '_Color': ({:.2f}, {:.2f}, {:.2f}, [1])", emissive.r, emissive.g, emissive.b);
            unityMaterial->SetColor("_Color", Color(emissive.r, emissive.g, emissive.b, 1));
        }

        // Zero metallic on the Standard shader just makes the material black
        if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Forcing '_Metallic' to 1");
        unityMaterial->SetFloat("_Metallic", 1);

        float roughnessFactor;
        if(material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor) == AI_SUCCESS) {
            if(LOG_A2U_MATERIAL_INFO) PaperLogger.info("Converting '$mat.roughnessFactor' to '_Glossiness': {:.2f} --> {:.2f}", roughnessFactor, 1 - roughnessFactor);
            unityMaterial->SetFloat("_Glossiness", 1 - roughnessFactor);
        }

        // All of these seem to need Standard keywords that don't have compiled varients
        // if(baseColor.a < 1 || material->Get(AI_MATKEY_TRANSMISSION_FACTOR, transmissionFactor) == AI_SUCCESS) {
        // if(getMaterialTextureIndex(material, aiTextureType_DIFFUSE_ROUGHNESS, diffuseRoughnessTextureIndex)) {
        // if(getMaterialTextureIndex(material, aiTextureType_NORMALS, normalsTextureIndex)) {
        // if(getMaterialTextureIndex(material, aiTextureType_HEIGHT, heightTextureIndex)) {
        // if(getMaterialTextureIndex(material, aiTextureType_AMBIENT_OCCLUSION, ambientOcclusionTextureIndex)) {

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
                if(LOG_A2U_NODE_DATA || LOG_A2U_MESH_INFO) PaperLogger.info("Combining mesh #: {}, Mesh: '{}'", i, unityMeshes[i]->get_name());
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

            if(LOG_A2U_NODE_DATA || LOG_A2U_MESH_INFO) PaperLogger.info("Pushing as mesh #: {}, Mesh: '{}'", unityMeshes.size(), meshName);
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
            if(LOG_A2U_NODE_DATA) PaperLogger.info("Material #: {}, Material: '{}'", materialIndex, targetMaterials[i]->get_name());
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
        unityTransform->set_localRotation(Quaternion::op_Multiply(Quaternion::op_Multiply(
            Quaternion::AngleAxis(nodeRotation.z / M_PI * 180, Vector3::get_forward()),
            Quaternion::AngleAxis(nodeRotation.y / M_PI * 180, Vector3::get_up())),
            Quaternion::AngleAxis(nodeRotation.x / M_PI * 180, Vector3::get_right())));
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