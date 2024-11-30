#include "main.hpp"

#include "scotland2/shared/modloader.h"

#include "custom-types/shared/register.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/PrimitiveType.hpp"
#include "UnityEngine/Resources.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "bsml/shared/BSML.hpp"

#include "particleController.hpp"
#include "testMenuViewController.hpp"

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


void LogMinMaxCurve(UnityEngine::ParticleSystem::MinMaxCurve& mmCurve) {
    if((int)mmCurve.m_Mode == 0) PaperLogger.info("Mode: Constant");
    else if((int)mmCurve.m_Mode == 1) PaperLogger.info("Mode: Curve");
    else if((int)mmCurve.m_Mode == 2) PaperLogger.info("Mode: TwoCurves");
    else if((int)mmCurve.m_Mode == 3) PaperLogger.info("Mode: TwoConstants");
    PaperLogger.info("CurveMultiplier: {}", mmCurve.m_CurveMultiplier);
    PaperLogger.info("CurveMin: [AnimationCurve]");
    PaperLogger.info("CurveMax: [AnimationCurve]");
    PaperLogger.info("ConstantMin: {}", mmCurve.m_ConstantMin);
    PaperLogger.info("ConstantMax: {}", mmCurve.m_ConstantMax);
}

void LogMinMaxCurve(Flare::ParticleController::MinMaxCurve& mmCurve) {
    if(mmCurve.mode == Flare::ParticleController::CurveMode::Constant) PaperLogger.info("Mode: Constant");
    else if(mmCurve.mode == Flare::ParticleController::CurveMode::Curve) PaperLogger.info("Mode: Curve");
    else if(mmCurve.mode == Flare::ParticleController::CurveMode::TwoCurves) PaperLogger.info("Mode: TwoCurves");
    else if(mmCurve.mode == Flare::ParticleController::CurveMode::TwoConstants) PaperLogger.info("Mode: TwoConstants");
    PaperLogger.info("CurveMultiplier: {}", mmCurve.curveMultiplier);
    PaperLogger.info("CurveMin: [AnimationCurve]");
    PaperLogger.info("CurveMax: [AnimationCurve]");
    PaperLogger.info("ConstantMin: {}", mmCurve.constantMin);
    PaperLogger.info("ConstantMax: {}", mmCurve.constantMax);
}

void LogMinMaxGradient(Flare::ParticleController::MinMaxGradient& mmGradient) {
    if(mmGradient.mode == Flare::ParticleController::GradientMode::Color) PaperLogger.info("Mode: Color");
    else if(mmGradient.mode == Flare::ParticleController::GradientMode::Gradient) PaperLogger.info("Mode: Gradient");
    else if(mmGradient.mode == Flare::ParticleController::GradientMode::TwoColors) PaperLogger.info("Mode: TwoColors");
    else if(mmGradient.mode == Flare::ParticleController::GradientMode::TwoGradients) PaperLogger.info("Mode: TwoGradients");
    else if(mmGradient.mode == Flare::ParticleController::GradientMode::RandomColor) PaperLogger.info("Mode: RandomColor");
    PaperLogger.info("GradientMin: [Gradient]");
    PaperLogger.info("GradientMax: [Gradient]");
    PaperLogger.info("ColorMin: ({}, {}, {}, {})", mmGradient.colorMin.r, mmGradient.colorMin.g, mmGradient.colorMin.b, mmGradient.colorMin.a);
    PaperLogger.info("ColorMax: ({}, {}, {}, {})", mmGradient.colorMax.r, mmGradient.colorMax.g, mmGradient.colorMax.b, mmGradient.colorMax.a);
}

MAKE_HOOK_MATCH(TestHook, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    TestHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if(!firstActivation) return;

    UnityEngine::GameObject* dustGo = UnityEngine::GameObject::Find("DustPS");
    UnityEngine::ParticleSystem* particles = dustGo->GetComponent<UnityEngine::ParticleSystem*>();

    PaperLogger.info("startSizeMultiplier: {}", Flare::ParticleController::MainModule::get_startSizeMultiplier(particles));
    Flare::ParticleController::MainModule::set_startSizeMultiplier(particles, Flare::ParticleController::MainModule::get_startSizeMultiplier(particles) * 0.0f);

    Flare::ParticleController::MinMaxCurve speed = Flare::ParticleController::MainModule::get_startSpeed(particles);
    LogMinMaxCurve(speed);
    speed.constantMin = 0.3f; // 0.02
    speed.constantMax = 1.0f; // 0.1
    Flare::ParticleController::MainModule::set_startSpeed(particles, speed);

    Flare::ParticleController::MinMaxGradient color = Flare::ParticleController::MainModule::get_startColor(particles);
    LogMinMaxGradient(color);
    color.mode = Flare::ParticleController::GradientMode::TwoColors;
    color.colorMin = UnityEngine::Color(0.0f, 1.0f, 0.0f, 1.0f);
    color.colorMax = UnityEngine::Color(1.0f, 0.0f, 0.0f, 1.0f);
    Flare::ParticleController::MainModule::set_startColor(particles, color);
    LogMinMaxGradient(color);
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
    BSML::Register::RegisterMainMenu<Flare::TestMenuViewController*>("Title", "Test button", "Hover hint");

    PaperLogger.info("Installing hooks...");

    INSTALL_HOOK(PaperLogger, TestHook);

    PaperLogger.info("Installed all hooks!");
}