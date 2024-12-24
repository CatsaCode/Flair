#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"

#include "bsml/shared/BSML.hpp"

#ifdef HotReload
    DECLARE_CLASS_CUSTOM(Flare, TestMenuViewController, BSML::HotReloadViewController,
#else
    DECLARE_CLASS_CODEGEN(Flare, TestMenuViewController, HMUI::ViewController,
#endif

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    
    DECLARE_INSTANCE_METHOD(void, PostParse);

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, positionXText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, positionYText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, positionZText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, sizeXText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, sizeYText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, sizeZText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, startLifetimeText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, startSizeText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, maxParticlesText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, gravityText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, rateOverTimeText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, rateOverDistanceText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, dampenText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, bounceText);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, lifetimeLossText);
);