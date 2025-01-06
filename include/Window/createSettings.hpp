#pragma once

#include "particleController.hpp"

#include "UnityEngine/GameObject.hpp"

namespace Flair::Window {

    UnityEngine::GameObject* CreateBoolSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange);
    UnityEngine::GameObject* CreateFloatSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, float startValue, std::function<void(float)> onChange);
    UnityEngine::GameObject* CreateMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHing, ParticleController::MinMaxCurve startValue, std::function<void(ParticleController::MinMaxCurve)> onChange);
    UnityEngine::GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleController::MinMaxGradient startValue, std::function<void(ParticleController::MinMaxGradient)> onChange);

}