#pragma once

#include "assimp/scene.h"

#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/Material.hpp"

#include <string>
#include <vector>

namespace Flair {

    class Project {
        private:
            void LoadMeshes(const aiScene* scene);
            void LoadTextures(const aiScene* scene);
            void LoadMaterials(const aiScene* scene);

        public:
            std::string name;
            std::string author;
            std::string description;

            std::vector<SafePtrUnity<UnityEngine::Mesh>> meshes;
            std::vector<SafePtrUnity<UnityEngine::Material>> materials;

            Project();
            Project(std::string_view filePath);

            void LoadFromFile(std::string_view filePath);
    };

}