#include "main.hpp"
#include "testMenuViewController.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

#include "particleController.hpp"

#include "UnityEngine/ParticleSystem.hpp"

#include <string>

DEFINE_TYPE(Flare, TestMenuViewController);

void Flare::TestMenuViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(!firstActivation) return;
    BSML::parse_and_construct(IncludedAssets::testMenu_bsml, transform, this);
    #ifdef HotReload
        fileWatcher->filePath = "sdcard/BSMLHotReload/Flare/testMenu.bsml";
        fileWatcher->checkInterval = 1.0f;
    #endif
}

void Flare::TestMenuViewController::PostParse() {
    PaperLogger.info("PostParse()");

    ApplySnowDust();
}