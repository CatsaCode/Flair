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


MAKE_HOOK_MATCH(TestHook, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    TestHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    PaperLogger.info("Loading file...");

    std::string file = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/testCube.glb";

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file, 0);

    if(scene == nullptr) {
        PaperLogger.info("Error!");
        PaperLogger.error("{}", importer.GetErrorString());
        return;
    }

    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        PaperLogger.info("Mesh name: {}", mesh->mName.C_Str());

        for(int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D vertex = mesh->mVertices[j];
            PaperLogger.info("X: {}, Y: {}, Z: {}", vertex.x, vertex.y, vertex.z);
        }
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

    INSTALL_HOOK(PaperLogger, TestHook);

    PaperLogger.info("Installed all hooks!");
}