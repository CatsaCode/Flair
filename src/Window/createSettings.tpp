#include "main.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/GameObject.hpp"

namespace Flair::Window {

    // Limit scope in a .tpp file
    using namespace UnityEngine;
    using namespace UnityEngine::UI;

    template<typename T> GameObject* CreateDropdownSetting(Transform* parent, std::string_view name, std::string_view hoverHint, const std::vector<std::string_view>& optionNames, const std::vector<T>& optionValues, T startValue, std::function<void(T)> onChange) {
        // Find which string is supposed to be shown from the given startValue
        std::string_view startValueString = optionNames[std::find(optionValues.begin(), optionValues.end(), startValue) - optionValues.begin()];

        // Set up the function that converts the selected string into a T for the provided onChange function
        std::function<void(StringW)> onChangeString = [optionNames, optionValues, onChange](StringW value){
            if(!onChange) return;
            onChange(optionValues[std::find(optionNames.begin(), optionNames.end(), value) - optionNames.begin()]);
        };

        // Cast the const vector reference to a read-only span for the BSML template
        std::span<std::string_view> optionNamesSpan = const_cast<std::vector<std::string_view>&>(optionNames);
        
        // Create dropdown from BSML template
        auto dropdownSetting = BSML::Lite::CreateDropdown(parent, name, startValueString, optionNamesSpan, onChangeString)->get_transform()->get_parent();
        Object::DestroyImmediate(dropdownSetting->GetComponent<LayoutElement*>());

        auto nameText = dropdownSetting->get_transform()->Find("Label")->GetComponent<HMUI::CurvedTextMeshPro*>();
        auto nameTransform = nameText->GetComponent<RectTransform*>();
        nameTransform->set_anchorMin({0, 0});
        nameTransform->set_anchorMax({1, 1});
        nameTransform->set_pivot({0, 0.5});
        nameTransform->set_anchoredPosition({0, 0});
        nameTransform->set_sizeDelta({0, 0});
        nameText->set_verticalAlignment(TMPro::VerticalAlignmentOptions::Middle);

        auto dropdownTransform = dropdownSetting->get_transform()->Find("Dropdown")->GetComponent<RectTransform*>();
        dropdownTransform->set_anchorMin({0.5, 0});
        dropdownTransform->set_anchorMax({1, 1});
        dropdownTransform->set_pivot({1, 0.5});
        dropdownTransform->set_anchoredPosition({0, 0});
        dropdownTransform->set_sizeDelta({0, 0});

        // Add a hover hint for more information
        if(!hoverHint.empty()) BSML::Lite::AddHoverHint(dropdownSetting, hoverHint);

        return dropdownSetting->get_gameObject();
    }

}

