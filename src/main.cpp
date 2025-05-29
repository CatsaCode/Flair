#include "main.hpp"

#include "scotland2/shared/modloader.h"
#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML.hpp"


// DEBUG
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "Window/window.hpp"
#include "Window/createModuleWindows.hpp"

#include "assimp/shared/assimp/Importer.hpp"
#include "assimp/shared/assimp/scene.h"
#include "assimp/shared/assimp/postprocess.h"
#include "assimp/shared/assimp/mesh.h"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshTopology.hpp"
#include "UnityEngine/Rendering/IndexFormat.hpp"

#include "bsml/shared/Helpers/getters.hpp"


static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};
// Stores the ID and version of our mod, and is sent to
// the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be
// removed if those are in use
Configuration &getConfig() {
    static Configuration config(modInfo);
    return config;
}


MAKE_HOOK_MATCH(AssimpTestHook, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    AssimpTestHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    PaperLogger.info("Loading file...");

    // std::string file = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/testCube.glb";
    std::string file = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/teapot.glb";

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file, 
        aiProcess_Triangulate);

    if(scene == nullptr) {
        PaperLogger.info("Error!");
        PaperLogger.error("{}", importer.GetErrorString());
        return;
    }

    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        PaperLogger.info("Mesh name: {}", mesh->mName.C_Str());
        
        UnityEngine::Mesh* unityMesh = UnityEngine::Mesh::New_ctor();
        unityMesh->set_name(mesh->mName.C_Str());
        unityMesh->Clear();

        ArrayW<UnityEngine::Vector3> vertices = ArrayW<UnityEngine::Vector3>(mesh->mNumVertices);
        for(int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D vertex = mesh->mVertices[j];
            PaperLogger.info("Vertex {} | X: {}, Y: {}, Z: {}", j, vertex.x, vertex.y, vertex.z);
            vertices[j] = UnityEngine::Vector3(vertex.x, vertex.y, vertex.z);
        }
        unityMesh->set_vertices(vertices);

        ArrayW<int> triangles = ArrayW<int>(mesh->mNumFaces * 3);
        for(int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for(int k = 0; k < 3; k++) {
                PaperLogger.info("Face {} Index {}", j, face.mIndices[k]);
                triangles[j * 3 + k] = face.mIndices[k];
            }
        }
        unityMesh->set_triangles(triangles);

        if(mesh->mNormals != nullptr) {
            ArrayW<UnityEngine::Vector3> normals = ArrayW<UnityEngine::Vector3>(mesh->mNumVertices);
            for(int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D normal = mesh->mNormals[j];
                PaperLogger.info("Normal {} | X: {}, Y: {}, Z: {}", j, normal.x, normal.y, normal.z);
                normals[j] = UnityEngine::Vector3(normal.x, normal.y, normal.z);
            }
            unityMesh->set_normals(normals);
        } else {
            unityMesh->RecalculateNormals();
        }



        UnityEngine::GameObject* testGO = UnityEngine::GameObject::New_ctor("FlairTest");
        testGO->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 0.5));
        testGO->get_transform()->set_localScale(UnityEngine::Vector3(0.2, 0.2, 0.2));

        UnityEngine::MeshFilter* testFilter = testGO->AddComponent<UnityEngine::MeshFilter*>();
        testFilter->set_sharedMesh(unityMesh);
        
        UnityEngine::MeshRenderer* testRenderer = testGO->AddComponent<UnityEngine::MeshRenderer*>();
        UnityEngine::Shader* shader = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Shader*>()->FirstOrDefault([](UnityEngine::Shader* shader){return shader->get_name() == "Standard";});
        UnityEngine::Material* material = UnityEngine::Material::New_ctor(shader);
        testRenderer->set_sharedMaterial(material);
    }



    // // if(!firstActivation) return;

    // static UnityEngine::ParticleSystem* dustParticleSystem = nullptr;

    // if(dustParticleSystem == nullptr) for(auto particle : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::ParticleSystem*>()) if(particle->name == "DustPS") {
    //     dustParticleSystem = particle;
    //     break;
    // }
    // if(dustParticleSystem == nullptr) {
    //     PaperLogger.error("Could not find DustPS");
    //     return;
    // }

    // if(rand() % 2 == 0) Flair::Window::CreateWindow(nullptr, "Test Window", {40, 100});
    // else Flair::Window::CreateMainModuleWindow(nullptr, dustParticleSystem);
}



// Called at the early stages of game loading
MOD_EXTERN_FUNC void setup(CModInfo *info) noexcept {
    *info = modInfo.to_c();

    getConfig().Load();

    // File logging
    Paper::Logger::RegisterFileContextId(PaperLogger.tag);

    PaperLogger.info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
MOD_EXTERN_FUNC void late_load() noexcept {
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();

    BSML::Init();

    // Set a seed for random numbers
    srand(time(nullptr));

    PaperLogger.info("Installing hooks...");

    INSTALL_HOOK(PaperLogger, AssimpTestHook);

    PaperLogger.info("Installed all hooks!");
}