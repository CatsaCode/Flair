#include "main.hpp"

#include "scotland2/shared/modloader.h"
#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML.hpp"


// DEBUG
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "Windows/mainModuleWindow.hpp"
#include "UnityEngine/Resources.hpp"


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

    if(!firstActivation) return;

    UnityEngine::ParticleSystem* dustParticleSystem = nullptr;
    for(auto particle : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::ParticleSystem*>()) if(particle->name == "DustPS") {
        dustParticleSystem = particle;
        break;
    }
    if(dustParticleSystem == nullptr) {
        PaperLogger.error("Could not find DustPS");
        return;
    }

    BSML::FloatingScreen* testFloatingScreen = BSML::FloatingScreen::CreateFloatingScreenWithViewcontroller<Flair::Windows::MainModuleWindow*>(UnityEngine::Vector2(100.0f, 100.0f), true, UnityEngine::Vector3(0.0f, 2.0f, 2.0f), UnityEngine::Quaternion::Euler(UnityEngine::Vector3(0.0f, 0.0f, 0.0f)), 0.0f, false);
    testFloatingScreen->set_HandleSide(BSML::Side::Top);
    Flair::Windows::MainModuleWindow* testWindow = reinterpret_cast<Flair::Windows::MainModuleWindow*>(testFloatingScreen->_rootViewController.ptr());
    testWindow->controlledParticleSystem = dustParticleSystem;
    // testWindow->RefreshUI();
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

    PaperLogger.info("Installing hooks...");

    INSTALL_HOOK(PaperLogger, TestHook);

    PaperLogger.info("Installed all hooks!");
}