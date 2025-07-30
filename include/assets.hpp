#pragma once

#include "assimp/mesh.h"
#include "assimp/texture.h"
#include "assimp/material.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Material.hpp"

namespace Flair::Assets {

    UnityEngine::GameObject* loadModel(std::string_view filePath);

    UnityEngine::Mesh* assimpToUnity(const aiMesh* mesh);
    UnityEngine::Texture2D* assimpToUnity(const aiTexture* texture);
    UnityEngine::Material* assimpToUnity(const aiMaterial* material, std::vector<UnityEngine::Texture2D*>& unityTextures, std::string_view filePath);
    UnityEngine::GameObject* assimpToUnity(const aiNode* node, std::vector<UnityEngine::Mesh*>& unityMeshes, std::vector<std::vector<int>>& submeshIndices, const std::vector<UnityEngine::Material*>& unityMaterials, const std::vector<int>& meshMaterialIndices);

}