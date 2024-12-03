#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"

#ifdef HotReload
    DECLARE_CLASS_CUSTOM(Flare, TestMenuViewController, BSML::HotReloadViewController,
#else
    DECLARE_CLASS_CODEGEN(Flare, TestMenuViewController, HMUI::ViewController,
#endif

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
);