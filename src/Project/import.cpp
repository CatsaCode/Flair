#include "project.hpp"
#include "main.hpp"
#include "logging.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"

#include "UnityEngine/Resources.hpp" // DEBUG

#include <cstring>
#include <exception>
#include <string_view>

using namespace Flair;
using namespace UnityEngine;

void Project::importFromFile(std::string_view filePath) {
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

    if(LOG_SCENE_HIERARCHY) logHierarchy(scene->mRootNode);

    importMeshes(scene);
    importTextures(scene);
    importMaterials(scene);
    importPrefabs(scene);
}

void Project::importMeshes(const aiScene* scene) {
    if(LOG_MESH_DATA) PaperLogger.info("Loading meshes...");
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

        if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Material #: {}", mesh->mMaterialIndex);

        std::string pathName = "Meshes/";
        pathName += mesh->mName.C_Str();

        meshes.push_back({pathName, unityMesh});
    }
}

void Project::importTextures(const aiScene* scene) {
    if(LOG_TEXTURE_DATA) PaperLogger.info("Loading textures...");
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

        std::string pathName = "Textures/";
        pathName += texture->mFilename.C_Str();

        textures.push_back({pathName, unityTexture});
    }
}

void Project::importMaterials(const aiScene* scene) {
    if(LOG_MATERIAL_DATA) PaperLogger.info("Loading materials...");
    // DEBUG
    Shader* shader = Resources::FindObjectsOfTypeAll<Shader*>()->FirstOrDefault([](Shader* shader){return shader->get_name() == "Standard";});
    Material* material = Material::New_ctor(shader);

    if(textures.size() > 0) material->set_mainTexture(textures[0].second.ptr());

    std::string pathName = "Materials/";
    pathName += "debugDefault";

    materials.push_back({pathName, material});
}

void Project::importPrefabs(const aiScene* scene) {
    if(LOG_PREFAB_DATA) PaperLogger.info("Loading prefabs...");
    static SafePtrUnity<GameObject> flairProjectsGO = nullptr;
    if(!flairProjectsGO) {
        if(LOG_GARBAGE_COLLECTION_ISSUES && (long int)flairProjectsGO != 0) PaperLogger.info("The FlairProjects GameObject was GCed! Recreating...");
        flairProjectsGO = GameObject::New_ctor("FlairProjects");
        Object::DontDestroyOnLoad(flairProjectsGO.ptr());
        flairProjectsGO->SetActive(false);
    }
    Transform* flairProjectsTransform = flairProjectsGO->get_transform();

    aiNode* rootNode = scene->mRootNode;
    if(rootNode == nullptr) return;

    if(LOG_PREFAB_DATA) PaperLogger.info("Creating project \"{}\" GameObject...", "TODO ProjectName");
    GameObject* projectGO = GameObject::New_ctor("TODO ProjectName");
    Transform* projectTransform = projectGO->get_transform();
    projectTransform->SetParent(flairProjectsTransform, false);

    // TODO Assuming true
    bool newImport = scene->mMetaData == nullptr || !scene->mMetaData->HasKey("flair");

    bool rootIsPrefab = strcmp(rootNode->mName.C_Str(), "ROOT") != 0;
    if(LOG_PREFAB_DATA) PaperLogger.info("Root node is prefab: {}", rootIsPrefab);
    if(rootIsPrefab) {
        GameObject* prefab = nodeToGameObject(scene, rootNode, projectTransform, newImport, DEPTH_STR);
        std::string pathName = "Prefabs/" + prefab->get_name();
        prefabs.push_back({pathName, prefab});
    } else {
        for(int i = 0; i < rootNode->mNumChildren; i++) {
            aiNode* childNode = rootNode->mChildren[i];
            GameObject* prefab = nodeToGameObject(scene, childNode, projectTransform, newImport, DEPTH_STR);
            std::string pathName = "Prefabs/" + prefab->get_name();
            prefabs.push_back({pathName, prefab});
        }
    }
}

void Project::setupMeshFilter(const aiScene* scene, const aiNode* node, GameObject* unityGO, const std::string& depthStr) {
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- MeshFilter component", depthStr);
    aiMetadata nodeMeshFilter;
    node->mMetaData->Get("meshFilter", nodeMeshFilter);

    int meshIndex;
    if(!nodeMeshFilter.Get("meshIndex", meshIndex)) throw fmt::format("Node \"{}\" has a meshFilter but is missing meshIndex!", node->mName.C_Str());
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- - Mesh #: {}", depthStr, meshIndex);
    
    if(meshIndex < 0) throw fmt::format("Node \"{}\" references mesh index {}, an invalid negative number!", node->mName.C_Str(), meshIndex);
    if(meshIndex >= meshes.size()) throw fmt::format("Node \"{}\" references mesh index {} but the project only contains {} meshes!", node->mName.C_Str(), meshIndex, meshes.size());
    if(!meshes[meshIndex].second) throw fmt::format("Mesh index {} has unloaded! TODO Reload.", meshIndex);
    
    MeshFilter* unityFilter = unityGO->AddComponent<MeshFilter*>();
    unityFilter->set_sharedMesh(meshes[meshIndex].second.ptr());
}

void Project::setupMeshRenderer(const aiScene* scene, const aiNode* node, GameObject* unityGO, const std::string& depthStr) {
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- MeshRenderer component", depthStr);
    aiMetadata nodeMeshRenderer;
    node->mMetaData->Get("meshRenderer", nodeMeshRenderer);

    int materialIndex;
    if(!nodeMeshRenderer.Get("materialIndex", materialIndex)) throw fmt::format("Node \"{}\" has a meshRenderer but is missing materialIndex!", node->mName.C_Str());
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- - Material #: {}", depthStr, materialIndex);
    
    if(materialIndex < 0) return;
    if(materialIndex >= materials.size()) throw fmt::format("Node \"{}\" references material index {} but the project only contains {} materials!", node->mName.C_Str(), materialIndex, materials.size());
    if(!materials[materialIndex].second) throw fmt::format("Material index {} has unloaded! TODO Reload.", materialIndex);
    
    MeshRenderer* unityRenderer = unityGO->AddComponent<MeshRenderer*>();
    unityRenderer->set_sharedMaterial(materials[materialIndex].second.ptr());
}

void Project::setupDefaultMeshFilter(const aiScene* scene, const aiNode* node, GameObject* unityGO, const std::string& depthStr) {
    int meshIndex = node->mMeshes[0];
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- - Mesh #: {}", depthStr, meshIndex);

    if(meshIndex < 0) throw fmt::format("Node \"{}\" references mesh index {}, an invalid negative number!", node->mName.C_Str(), meshIndex);
    if(meshIndex >= meshes.size()) throw fmt::format("Node \"{}\" references mesh index {} but the project only contains {} meshes!", node->mName.C_Str(), meshIndex, meshes.size());
    if(!meshes[meshIndex].second) throw fmt::format("Mesh index {} has unloaded! TODO Reload.", meshIndex);
    
    MeshFilter* unityFilter = unityGO->AddComponent<MeshFilter*>();
    unityFilter->set_sharedMesh(meshes[meshIndex].second.ptr());
}

void Project::setupDefaultMeshRenderer(const aiScene* scene, const aiNode* node, GameObject* unityGO, const std::string& depthStr) {
    int materialIndex = scene->mMeshes[node->mMeshes[0]]->mMaterialIndex;
    if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- - Material #: {}", depthStr, materialIndex);

    materialIndex = 0; // DEBUG

    if(materialIndex < 0) return;
    if(materialIndex >= materials.size()) throw fmt::format("Node \"{}\" references material index {} but the project only contains {} materials!", node->mName.C_Str(), materialIndex, materials.size());
    if(!materials[materialIndex].second) throw fmt::format("Material index {} has unloaded! TODO Reload.", materialIndex);

    MeshRenderer* unityRenderer = unityGO->AddComponent<MeshRenderer*>();
    unityRenderer->set_sharedMaterial(materials[materialIndex].second.ptr());
}

GameObject* Project::nodeToGameObject(const aiScene* scene, const aiNode* node, Transform* parent, const bool newImport, const std::string& depthStr/* = ""*/) {
    if(LOG_NODE_TO_GAMEOBJECT) {
        PaperLogger.info("{}", depthStr);
        PaperLogger.info("{}Name: {}", depthStr, node->mName.C_Str());
    }
    GameObject* unityGO = GameObject::New_ctor(node->mName.C_Str());

    Transform* unityTransform = unityGO->get_transform();
    unityTransform->SetParent(parent, false);
    aiVector3D nodePosition;
    aiVector3D nodeRotation;
    aiVector3D nodeScale;
    node->mTransformation.Decompose(nodeScale, nodeRotation, nodePosition);
    if(LOG_NODE_TO_GAMEOBJECT) {
        PaperLogger.info("{}- Local position: ({:.1f}, {:.1f}, {:.1f})", depthStr, nodePosition.x, nodePosition.y, nodePosition.z);
        PaperLogger.info("{}- Local rotation: ({:.1f}, {:.1f}, {:.1f})", depthStr, nodeRotation.x, nodeRotation.y, nodeRotation.z);
        PaperLogger.info("{}- Local scale: ({:.1f}, {:.1f}, {:.1f})", depthStr, nodeScale.x, nodeScale.y, nodeScale.z);
    }
    unityTransform->set_localPosition(Vector3(nodePosition.x, nodePosition.y, nodePosition.z));
    unityTransform->set_localEulerAngles(Vector3(nodeRotation.x, nodeRotation.y, nodeRotation.z));
    unityTransform->set_localScale(Vector3(nodeScale.x, nodeScale.y, nodeScale.z));

    try {
        if(!newImport && node->mMetaData != nullptr) {
            if(node->mMetaData->HasKey("meshFilter")) setupMeshFilter(scene, node, unityGO, depthStr);
            if(node->mMetaData->HasKey("meshRenderer")) setupMeshRenderer(scene, node, unityGO, depthStr);
            
        } else if(newImport) {
            if(node->mNumMeshes > 0) {
                if(LOG_NODE_TO_GAMEOBJECT) PaperLogger.info("{}- Newly imported mesh", depthStr, node->mNumMeshes);
                setupDefaultMeshFilter(scene, node, unityGO, depthStr);
                setupDefaultMeshRenderer(scene, node, unityGO, depthStr);
            }
        }
    } catch(const std::exception& err) {
        PaperLogger.error("{} Skipping components...", err.what());
    }

    for(int i = 0; i < node->mNumChildren; i++) {
        aiNode* child = node->mChildren[i];
        nodeToGameObject(scene, child, unityTransform, newImport, depthStr + DEPTH_STR);
    }

    return unityGO;
}