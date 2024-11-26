#include "main.hpp"

#include "scotland2/shared/modloader.h"
#include "custom-types/shared/register.hpp"

#include "bsml/shared/BSML/MainThreadScheduler.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Selectable.hpp"

#include "CustomTypes/particleController.hpp"

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

void Test() {
    auto transforms = UnityEngine::GameObject::FindObjectsOfType<UnityEngine::GameObject*>();
    for(int i = 0; i < transforms.size(); i++) if(transforms[i]->gameObject->GetComponent<Flare::ParticleController*>() == nullptr) transforms[i]->gameObject->AddComponent<Flare::ParticleController*>();

    BSML::MainThreadScheduler::ScheduleAfterTime(1.0f, Test);
}


MAKE_HOOK_MATCH(
    CreateSomethingHook, 
    &GlobalNamespace::MainMenuViewController::DidActivate, 
    void, 
    GlobalNamespace::MainMenuViewController* self,
    bool firstActivation, 
    bool addedToHierarchy, 
    bool screenSystemEnabling
) {
    CreateSomethingHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if(!firstActivation) return;

    self->gameObject->AddComponent<Flare::ParticleController*>();

    Test();
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

    PaperLogger.info("Installing hooks...");

    INSTALL_HOOK(PaperLogger, CreateSomethingHook);

    PaperLogger.info("Installed all hooks!");
}