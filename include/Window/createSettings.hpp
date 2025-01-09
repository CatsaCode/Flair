#pragma once

#include "particleInterface.hpp"

#include "UnityEngine/GameObject.hpp"

namespace Flair::Window {

    UnityEngine::GameObject* CreateBoolSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange);
    UnityEngine::GameObject* CreateFloatSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, float startValue, std::function<void(float)> onChange);
    UnityEngine::GameObject* CreateMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHing, ParticleInterface::MinMaxCurve startValue, std::function<void(ParticleInterface::MinMaxCurve)> onChange);
    UnityEngine::GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxGradient startValue, std::function<void(ParticleInterface::MinMaxGradient)> onChange);

}