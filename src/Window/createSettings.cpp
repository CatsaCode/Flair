#include "Window/createSettings.hpp"
#include "main.hpp"

#include "particleController.hpp"

#include "bsml/shared/BSML.hpp"

#include <sstream>

using namespace UnityEngine;

namespace Flair::Window {

    // Create a toggle setting for a window
    GameObject* CreateBoolSetting(Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange) {
        // Create object from BSML template
        auto boolSetting = BSML::Lite::CreateToggle(parent, name, startValue, onChange);

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(boolSetting, hoverHint);

        return boolSetting->get_gameObject();
    }

    // Create a float setting for a window
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

        // Create object from BSML template
        auto floatSetting = BSML::Lite::CreateStringSetting(parent, name, startValueSS.str(), onChangeString);

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(floatSetting, hoverHint);

        return floatSetting->get_gameObject();
    }

    // Create a MinMaxCurve setting for a window
    GameObject* CreateMinMaxCurveSetting(Transform* parent, std::string name, std::string hoverHint, ParticleController::MinMaxCurve startValue, std::function<void(ParticleController::MinMaxCurve value)> onChange) {
        // TODO Create a MinMaxCurveWindow
        
        // Convert startValue to a string without the trailing zeros
        std::stringstream startValueSS;
        startValueSS << startValue.constantMax;

        // Set up the function that converts the typed string into a MinMaxCurve for the provided onChange function
        std::function<void(std::string)> onChangeString = [onChange](std::string value){
            if(!onChange) return;
            if(value.empty()) value = "0";
            try {onChange(std::stof(value));}
            catch(...) {}; // TODO Make an actual numpad
        };

        // Create object from BSML template
        auto minMaxCurveSetting = BSML::Lite::CreateStringSetting(parent, name, startValueSS.str(), onChangeString);

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(minMaxCurveSetting, hoverHint);

        return minMaxCurveSetting->get_gameObject();
    }

    // Create a MinMaxGradient setting for a window
    GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleController::MinMaxGradient startValue, std::function<void(ParticleController::MinMaxGradient)> onChange) {
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