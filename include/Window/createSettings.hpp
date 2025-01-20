#pragma once

#include "particleInterface.hpp"

#include "UnityEngine/GameObject.hpp"

namespace Flair::Window {

    UnityEngine::GameObject* CreateBoolSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange);
    UnityEngine::GameObject* CreateIntSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, int startValue, std::function<void(int)> onChange);
    UnityEngine::GameObject* CreateFloatSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, float startValue, std::function<void(float)> onChange);
    template<typename T> UnityEngine::GameObject* CreateDropdownSetting(UnityEngine::Transform* parent, std::string_view name, std::string_view hoverHint, const std::vector<std::string_view>& optionNames, const std::vector<T>& optionValues, T startValue, std::function<void(T)> onChange);
    UnityEngine::GameObject* CreateMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValue, std::function<void(ParticleInterface::MinMaxCurve)> onChange);
    UnityEngine::GameObject* Create3DMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValueX, std::function<void(ParticleInterface::MinMaxCurve)> onChangeX, ParticleInterface::MinMaxCurve startValueY, std::function<void(ParticleInterface::MinMaxCurve)> onChangeY, ParticleInterface::MinMaxCurve startValueZ, std::function<void(ParticleInterface::MinMaxCurve)> onChangeZ);
    UnityEngine::GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxGradient startValue, std::function<void(ParticleInterface::MinMaxGradient)> onChange);

}

#include "../src/Window/createSettings.tpp"