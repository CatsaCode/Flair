#pragma once

#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"

#include "UnityEngine/ParticleSystem.hpp"
#include "bsml/shared/BSML.hpp"

#ifdef HOT_RELOAD
    DECLARE_CLASS_CUSTOM(Flair::Windows, MainModuleWindow, BSML::HotReloadViewController, 
#else
    DECLARE_CLASS_CODEGEN(Flair::Windows, MainModuleWindow, HMUI::ViewController,
#endif

    DECLARE_INSTANCE_FIELD(UnityEngine::ParticleSystem*, controlledParticleSystem);

    DECLARE_BSML_PROPERTY(StringW, durationValue);
    DECLARE_BSML_PROPERTY(bool, loopValue);
    DECLARE_BSML_PROPERTY(bool, prewarmValue);
    DECLARE_BSML_PROPERTY(StringW, startDelayValue);
    DECLARE_BSML_PROPERTY(StringW, startLifetimeValue);
    DECLARE_BSML_PROPERTY(StringW, startSpeedValue);
    DECLARE_BSML_PROPERTY(bool, startSize3DValue);
    DECLARE_BSML_PROPERTY(StringW, startSizeValue);
    DECLARE_BSML_PROPERTY(StringW, startSizeXValue);
    DECLARE_BSML_PROPERTY(StringW, startSizeYValue);
    DECLARE_BSML_PROPERTY(StringW, startSizeZValue);
    DECLARE_BSML_PROPERTY(bool, startRotation3DValue);
    DECLARE_BSML_PROPERTY(StringW, startRotationValue);
    DECLARE_BSML_PROPERTY(StringW, startRotationXValue);
    DECLARE_BSML_PROPERTY(StringW, startRotationYValue);
    DECLARE_BSML_PROPERTY(StringW, startRotationZValue);
    DECLARE_BSML_PROPERTY(StringW, flipRotationValue);
    DECLARE_BSML_PROPERTY(UnityEngine::Color, startColorValue);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

);