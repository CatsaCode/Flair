#include "main.hpp"

#include "logging.hpp"
#include "scotland2/shared/modloader.h"
#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML.hpp"


// DEBUG
#include "metacore/shared/events.hpp"
#include "metacore/shared/input.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "Window/window.hpp"
#include "Window/createModuleWindows.hpp"
#include "assets.hpp"

#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"


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

void SpawnToyota() {
    static UnityW<UnityEngine::GameObject> toyotaGO = nullptr;
    
    if(toyotaGO) {
        toyotaGO->SetActive(false);
        toyotaGO = nullptr;
        return;
    }

    PaperLogger.info("Loading file...");
    std::string filePath = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/toyota/source/toyota.glb";
    toyotaGO = Flair::Assets::loadModel(filePath);
    if(!toyotaGO) return;

    toyotaGO->get_transform()->set_position(UnityEngine::Vector3(0, 0.21, 1.5));
    toyotaGO->get_transform()->set_localScale(::UnityEngine::Vector3(0.4, 0.4, 0.4));
}


MAKE_HOOK_MATCH(AssimpTestHook, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    AssimpTestHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    logHierarchy(self->get_transform(), true, 1000, 1000);

    if(!firstActivation) return;

    MetaCore::Events::AddCallback(MetaCore::Input::ButtonEvents, MetaCore::Input::Buttons::BY, [](){
        static bool prevPressed = false;
        bool pressed = MetaCore::Input::GetPressed(MetaCore::Input::Controllers::Right, MetaCore::Input::Buttons::AX);
        if(pressed && !prevPressed) SpawnToyota();
        prevPressed = pressed;
    });

    // PaperLogger.info("Loading file...");

    // std::string filePath = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/toyota/source/toyota.glb";
    // // std::string file = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/teapot.glb";

    // UnityEngine::GameObject* testGO = Flair::Assets::loadModel(filePath);
    // if(!testGO) return;
    // // testGO->SetActive(true);
    // // testGO->get_transform()->set_position(UnityEngine::Vector3(0, 0.5, 0.5));
    // testGO->get_transform()->set_position(UnityEngine::Vector3(0, 0.21, 1.5));
    // testGO->get_transform()->set_localScale(::UnityEngine::Vector3(0.4, 0.4, 0.4));


    // std::string exportFilePath = "/storage/emulated/0/ModData/com.beatgames.beatsaber/Mods/Flair/export.gltf";

    // project.exportToFile(exportFilePath);
    


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