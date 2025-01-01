#include "Windows/mainModuleWindow.hpp"
#include "main.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"
#include "particleController.hpp"
#include "UnityEngine/Color.hpp"

DEFINE_TYPE(Flair::Windows, MainModuleWindow);

using namespace Flair::Windows;
using namespace UnityEngine;

void MainModuleWindow::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(!firstActivation) return;

    BSML::parse_and_construct(IncludedAssets::mainModuleWindow_bsml, get_transform());

    #ifdef HOT_RELOAD
        fileWatcher->filePath = "sdcard/BSMLHotReload/Flair/mainModuleWindow.bsml";
        fileWatcher->checkInterval = 1.0f;
    #endif
}

#define DEFINE_BSML_BOOL_VALUE(name) \
    bool MainModuleWindow::get_##name##Value() { \
        if(!controlledParticleSystem) return false; \
        return ParticleController::MainModule::get_##name(controlledParticleSystem); \
    } \
    void MainModuleWindow::set_##name##Value(bool value) { \
        if(!controlledParticleSystem) return; \
        ParticleController::MainModule::set_##name(controlledParticleSystem, value); \
    }

#define DEFINE_BSML_FLOAT_VALUE(name) \
    StringW MainModuleWindow::get_##name##Value() { \
        if(!controlledParticleSystem) return "Error"; \
        return std::to_string(ParticleController::MainModule::get_##name(controlledParticleSystem)); \
    } \
    void MainModuleWindow::set_##name##Value(StringW value) { \
        if(!controlledParticleSystem) return; \
        ParticleController::MainModule::set_##name(controlledParticleSystem, std::stof(static_cast<std::string>(value))); \
    }

#define DEFINE_BSML_MINMAXCURVE_VALUE(name) \
    StringW MainModuleWindow::get_##name##Value() { \
        if(!controlledParticleSystem) return "Error"; \
        return std::to_string(ParticleController::MainModule::get_##name(controlledParticleSystem).constantMax); \
    } \
    void MainModuleWindow::set_##name##Value(StringW value) { \
        if(!controlledParticleSystem) return; \
        ParticleController::MainModule::set_##name(controlledParticleSystem, std::stof(static_cast<std::string>(value))); \
    }

#define DEFINE_BSML_MINMAXGRADIENT_VALUE(name) \
    Color MainModuleWindow::get_##name##Value() { \
        if(!controlledParticleSystem) return Color(0, 0, 0, 1); \
        return ParticleController::MainModule::get_##name(controlledParticleSystem).colorMax; \
    } \
    void MainModuleWindow::set_##name##Value(Color value) { \
        if(!controlledParticleSystem) return; \
        ParticleController::MainModule::set_##name(controlledParticleSystem, value); \
    }

DEFINE_BSML_FLOAT_VALUE(duration);
DEFINE_BSML_BOOL_VALUE(loop);
DEFINE_BSML_BOOL_VALUE(prewarm);
DEFINE_BSML_MINMAXCURVE_VALUE(startDelay);
DEFINE_BSML_MINMAXCURVE_VALUE(startLifetime);
DEFINE_BSML_MINMAXCURVE_VALUE(startSpeed);
DEFINE_BSML_BOOL_VALUE(startSize3D);
DEFINE_BSML_MINMAXCURVE_VALUE(startSize);
DEFINE_BSML_MINMAXCURVE_VALUE(startSizeX);
DEFINE_BSML_MINMAXCURVE_VALUE(startSizeY);
DEFINE_BSML_MINMAXCURVE_VALUE(startSizeZ);
DEFINE_BSML_BOOL_VALUE(startRotation3D);
DEFINE_BSML_MINMAXCURVE_VALUE(startRotation);
DEFINE_BSML_MINMAXCURVE_VALUE(startRotationX);
DEFINE_BSML_MINMAXCURVE_VALUE(startRotationY);
DEFINE_BSML_MINMAXCURVE_VALUE(startRotationZ);
DEFINE_BSML_FLOAT_VALUE(flipRotation);
DEFINE_BSML_MINMAXGRADIENT_VALUE(startColor);