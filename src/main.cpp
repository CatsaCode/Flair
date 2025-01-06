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

    // if(!firstActivation) return;

    static UnityEngine::ParticleSystem* dustParticleSystem = nullptr;

    if(dustParticleSystem == nullptr) for(auto particle : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::ParticleSystem*>()) if(particle->name == "DustPS") {
        dustParticleSystem = particle;
        break;
    }
    if(dustParticleSystem == nullptr) {
        PaperLogger.error("Could not find DustPS");
        return;
    }

    if(rand() % 2 == 0) Flair::Window::CreateWindow(nullptr, "Test Window", {40, 100});
    else Flair::Window::CreateMainModuleWindow(nullptr, dustParticleSystem);
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