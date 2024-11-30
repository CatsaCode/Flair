#include "main.hpp"
#include "testMenuViewController.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(Flare, TestMenuViewController);

void Flare::TestMenuViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(!firstActivation) return;
    BSML::parse_and_construct(IncludedAssets::testMenu_bsml, transform, this);
    fileWatcher->filePath = "sdcard/BSMLHotReload/Flare/testMenu.bsml";
}