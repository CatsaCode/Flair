#pragma once

#include "assimp/scene.h"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Material.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Flair {

    struct Instance {
        std::string name;

        UnityEngine::Vector3 position;
        UnityEngine::Vector3 rotation;
        UnityEngine::Vector3 scale;

        std::vector<std::string> requireTags;
        std::vector<std::string> filterTags;
    };

    struct Prefab {
        SafePtrUnity<UnityEngine::GameObject> go;
        std::vector<Instance> instances;
    };

    class Project {
        private:
            // void importMeshes(const aiScene* scene);
            // void importTextures(const aiScene* scene);
            // void importMaterials(const aiScene* scene);
            // void importPrefabs(const aiScene* scene);

            // void setupMeshFilter(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            // void setupMeshRenderer(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            // void setupDefaultMeshFilter(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            // void setupDefaultMeshRenderer(const aiScene* scene, const aiNode* node, UnityEngine::GameObject* unityGO, const std::string& depthStr);
            // UnityEngine::GameObject* nodeToGameObject(const aiScene* scene, const aiNode* node, UnityEngine::Transform* parent, const bool newImport, const std::string& depthStr = "");

            // void exportMeshes(aiScene& scene);
            // void exportMaterials(aiScene& scene);

        public:
            std::string name;
            std::string author;
            std::string description;

            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Mesh>>> meshes;
            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Texture>>> textures;
            std::vector<std::pair<std::string, SafePtrUnity<UnityEngine::Material>>> materials;
            std::vector<std::pair<std::string, std::unique_ptr<Prefab>>> prefabs;

            Project() {}
            // Project(std::string_view filePath) {
            //     importFromFile(filePath);
            // }

            // void importFromFile(std::string_view filePath);
            // void exportToFile(std::string_view filePath);
    };

}