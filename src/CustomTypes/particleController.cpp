#include "main.hpp"
#include "CustomTypes/particleController.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Space.hpp"

DEFINE_TYPE(Flare, ParticleController);

void Flare::ParticleController::Update() {
    double power = pow(UnityEngine::Time::get_realtimeSinceStartup(), 4.0) * 0.0000000001;
    transform->Translate(
        ((rand() % 1000) / 1000.0f - 0.5f) * power,
        ((rand() % 1000) / 1000.0f - 0.5f) * power,
        ((rand() % 1000) / 1000.0f - 0.5f) * power,
        UnityEngine::Space::World
    );
}