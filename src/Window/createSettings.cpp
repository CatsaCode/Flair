#include "Window/createSettings.hpp"
#include "main.hpp"

#include "particleInterface.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

#include <sstream>

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace Flair::Window {

    // Create a toggle setting for a Window
    GameObject* CreateBoolSetting(Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange) {
        // Create object from BSML template
        auto boolSetting = BSML::Lite::CreateToggle(parent, name, startValue, onChange);

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(boolSetting, hoverHint);

        return boolSetting->get_gameObject();
    }

    // Create an int setting for a Window
    GameObject* CreateIntSetting(Transform* parent, std::string name, std::string hoverHint, int startValue, std::function<void(int)> onChange) {
        // Convert startValue to a string
        std::string startValueStr = fmt::format("{}", startValue);
        
        // Set up the function that converts the typed string into a float for the provided onChange function
        std::function<void(std::string)> onChangeString = [onChange](std::string value){
            if(!onChange) return;
            if(value.empty()) value = "0";
            try {onChange(std::stoi(value));}
            catch(...) {}; // TODO Make an actual numpad
        };

        // Create container for the nameText and stringSetting
        auto intSetting = GameObject::New_ctor("IntSetting");
        intSetting->get_transform()->SetParent(parent, false);
        auto intSettingTransform = intSetting->AddComponent<RectTransform*>();
        intSettingTransform->set_sizeDelta({0, 8}); // Set specific height for the VerticalLayoutGroup

        if(!name.empty()) {
            // Create text from BSML template
            auto nameText = BSML::Lite::CreateText(intSetting->get_transform(), name, {0, 0}, {0, 0});
            // Control layout with RectTransform
            Object::DestroyImmediate(nameText->GetComponent<LayoutElement*>());
            auto nameTextTransform = nameText->GetComponent<RectTransform*>();
            nameTextTransform->set_anchorMin({0, 0});
            nameTextTransform->set_anchorMax({1, 1});
            nameTextTransform->set_pivot({0, 0.5});
            nameTextTransform->set_anchoredPosition({0, 0});
            nameTextTransform->set_sizeDelta({0, 0});
            nameText->set_verticalAlignment(TMPro::VerticalAlignmentOptions::Middle);
        }

        // Create string setting from BSML template
        auto stringSetting = BSML::Lite::CreateStringSetting(intSetting->get_transform(), "0", startValueStr, onChangeString);
        Object::DestroyImmediate(stringSetting->GetComponent<LayoutElement*>()); // Don't let it override the width
        auto stringSettingTransform = stringSetting->GetComponent<RectTransform*>();
        stringSettingTransform->set_anchorMin({0.5, 0});
        if(name.empty()) stringSettingTransform->set_anchorMin({0, 0}); // Take up full area if no text is given
        stringSettingTransform->set_anchorMax({1, 1});
        stringSettingTransform->set_pivot({1, 0.5});
        stringSettingTransform->set_anchoredPosition({0, 0});
        stringSettingTransform->set_sizeDelta({0, 0});

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(intSetting, hoverHint);

        return intSetting->get_gameObject();
    }

    // Create a float setting for a Window
    GameObject* CreateFloatSetting(Transform* parent, std::string name, std::string hoverHint, float startValue, std::function<void(float)> onChange) {
        // Convert startValue to a string without the trailing zeros
        std::stringstream startValueSS;
        startValueSS << startValue;

        // Set up the function that converts the typed string into a float for the provided onChange function
        std::function<void(std::string)> onChangeString = [onChange](std::string value){
            if(!onChange) return;
            if(value.empty()) value = "0";
            try {onChange(std::stof(value));}
            catch(...) {}; // TODO Make an actual numpad
        };

        // Create container for the nameText and stringSetting
        auto floatSetting = GameObject::New_ctor("FloatSetting");
        floatSetting->get_transform()->SetParent(parent, false);
        auto floatSettingTransform = floatSetting->AddComponent<RectTransform*>();
        floatSettingTransform->set_sizeDelta({0, 8}); // Set specific height for the VerticalLayoutGroup

        if(!name.empty()) {
            // Create text from BSML template
            auto nameText = BSML::Lite::CreateText(floatSetting->get_transform(), name, {0, 0}, {0, 0});
            // Control layout with RectTransform
            Object::DestroyImmediate(nameText->GetComponent<LayoutElement*>());
            auto nameTextTransform = nameText->GetComponent<RectTransform*>();
            nameTextTransform->set_anchorMin({0, 0});
            nameTextTransform->set_anchorMax({1, 1});
            nameTextTransform->set_pivot({0, 0.5});
            nameTextTransform->set_anchoredPosition({0, 0});
            nameTextTransform->set_sizeDelta({0, 0});
            nameText->set_verticalAlignment(TMPro::VerticalAlignmentOptions::Middle);
        }

        // Create string setting from BSML template
        auto stringSetting = BSML::Lite::CreateStringSetting(floatSetting->get_transform(), "0.0", startValueSS.str(), onChangeString);
        Object::DestroyImmediate(stringSetting->GetComponent<LayoutElement*>()); // Don't let it override the width
        auto stringSettingTransform = stringSetting->GetComponent<RectTransform*>();
        stringSettingTransform->set_anchorMin({0.5, 0});
        if(name.empty()) stringSettingTransform->set_anchorMin({0, 0}); // Take up full area if no text is given
        stringSettingTransform->set_anchorMax({1, 1});
        stringSettingTransform->set_pivot({1, 0.5});
        stringSettingTransform->set_anchoredPosition({0, 0});
        stringSettingTransform->set_sizeDelta({0, 0});

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(floatSetting, hoverHint);

        return floatSetting->get_gameObject();
    }

    // Create a MinMaxCurve setting for a Window
    GameObject* CreateMinMaxCurveSetting(Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValue, std::function<void(ParticleInterface::MinMaxCurve value)> onChange) {
        // TODO Create a MinMaxCurveWindow

        // Set up the function that converts the typed float into a MinMaxCurve for the provided onChange function
        std::function<void(float)> onChangeFloat = [onChange](float value) {
            if(!onChange) return;
            onChange(value);
        };

        // Create temporary object from CreateFloatSetting
        auto minMaxCurveSetting = CreateFloatSetting(parent, name, hoverHint, startValue.constantMax, onChangeFloat);

        return minMaxCurveSetting->get_gameObject();
    }

    // Create an X, Y, and Z MinMaxCurve setting for a Window
    GameObject* Create3DMinMaxCurveSetting(Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValueX, std::function<void(ParticleInterface::MinMaxCurve)> onChangeX, ParticleInterface::MinMaxCurve startValueY, std::function<void(ParticleInterface::MinMaxCurve)> onChangeY, ParticleInterface::MinMaxCurve startValueZ, std::function<void(ParticleInterface::MinMaxCurve)> onChangeZ) {
        // Create a container for the three MinMaxCurveSettings
        auto row = GameObject::New_ctor("3DMinMaxCurveSetting");
        row->get_transform()->SetParent(parent->get_transform(), false);
        auto rowTransform = row->AddComponent<RectTransform*>();
        // Set a constant height, same as the BSML string input
        rowTransform->set_sizeDelta({0, 8});
        // Evenly space child objects, don't try to control their height
        auto rowLayout = row->AddComponent<HorizontalLayoutGroup*>();
        rowLayout->set_childControlWidth(true);
        rowLayout->set_childControlHeight(false);

        // TODO Add name

        // Add each of the MinMaxCurveSettings to the row
        CreateMinMaxCurveSetting(row->get_transform(), "", "", startValueX, onChangeX);
        CreateMinMaxCurveSetting(row->get_transform(), "", "", startValueY, onChangeY);
        CreateMinMaxCurveSetting(row->get_transform(), "", "", startValueZ, onChangeZ);

        return row;
    }

    // Create a MinMaxGradient setting for a Window
    GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxGradient startValue, std::function<void(ParticleInterface::MinMaxGradient)> onChange) {
        // TODO Create a MinMaxGradientWindow

        // Set up the function that converts the single color into a MinMaxCurve for the provided onChange function
        std::function<void(Color)> onChangeColor = [onChange](Color value){
            if(!onChange) return;
            onChange(value);
        };

        // Create object from BSML template
        auto minMaxGradientSetting = BSML::Lite::CreateColorPicker(parent, name, startValue.colorMax, nullptr, nullptr, onChangeColor);

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(minMaxGradientSetting, hoverHint);

        return minMaxGradientSetting->get_gameObject();
    }

}