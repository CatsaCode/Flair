#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ViewController.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"

DECLARE_CLASS_CODEGEN(Flare, TestMenuViewController, BSML::HotReloadViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
);