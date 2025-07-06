#pragma once

#include "assimp/scene.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Material.hpp"

#include <string>
#include <vector>

namespace Flair {

    class Project {
        private:
            void importMeshes(const aiScene* scene);
            void importTextures(const aiScene* scene);
            void importMaterials(const aiScene* scene);
            void importPrefabs(const aiScene* scene);

            void setupMeshFilter(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            void setupMeshRenderer(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            void setupDefaultMeshFilter(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            void setupDefaultMeshRenderer(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            UnityEngine::GameObject* nodeToGameObject(const aiScene* scene, const aiNode* node, UnityEngine::Transform* parent, const bool newImport, const std::string& depthStr = "");

            void exportMeshes(aiScene& scene);
            void exportMaterials(aiScene& scene);

        public:
            std::string name;
            std::string author;
            std::string description;

            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Mesh>>> meshes;
            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Texture>>> textures;
            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Material>>> materials;
            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::GameObject>>> prefabs;

            Project() {}
            Project(std::string_view filePath) {
                importFromFile(filePath);
            }

            void importFromFile(std::string_view filePath);
            void exportToFile(std::string_view filePath);
    };

}