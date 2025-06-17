#pragma once

#include "assimp/scene.h"

#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Material.hpp"

#include <string>
#include <vector>

namespace Flair {

    class Project {
        private:
            void LoadMeshes(const aiScene* scene);
            void LoadTextures(const aiScene* scene);
            void LoadMaterials(const aiScene* scene);
            void LoadPrefabs(const aiScene* scene);

            UnityEngine::GameObject* NodeToGameObject(const aiScene* scene, const aiNode* node, UnityEngine::Transform* parent, const bool newImport, const int depth = 0);

        public:
            std::string name;
            std::string author;
            std::string description;

            std::vector<SafePtrUnity<UnityEngine::Mesh>> meshes;
            std::vector<SafePtrUnity<UnityEngine::Texture>> textures;
            std::vector<SafePtrUnity<UnityEngine::Material>> materials;
            std::vector<SafePtrUnity<UnityEngine::GameObject>> prefabs;

            Project();
            Project(std::string_view filePath);

            void LoadFromFile(std::string_view filePath);
    };

}