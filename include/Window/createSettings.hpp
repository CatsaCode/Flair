#pragma once

#include "particleInterface.hpp"

#include "UnityEngine/GameObject.hpp"

namespace Flair::Window {

    UnityEngine::GameObject* CreateBoolSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, bool startValue, std::function<void(bool)> onChange);
    UnityEngine::GameObject* CreateFloatSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, float startValue, std::function<void(float)> onChange);
    UnityEngine::GameObject* CreateMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValue, std::function<void(ParticleInterface::MinMaxCurve)> onChange);
    UnityEngine::GameObject* Create3DMinMaxCurveSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxCurve startValueX, std::function<void(ParticleInterface::MinMaxCurve)> onChangeX, ParticleInterface::MinMaxCurve startValueY, std::function<void(ParticleInterface::MinMaxCurve)> onChangeY, ParticleInterface::MinMaxCurve startValueZ, std::function<void(ParticleInterface::MinMaxCurve)> onChangeZ);
    UnityEngine::GameObject* CreateMinMaxGradientSetting(UnityEngine::Transform* parent, std::string name, std::string hoverHint, ParticleInterface::MinMaxGradient startValue, std::function<void(ParticleInterface::MinMaxGradient)> onChange);

}