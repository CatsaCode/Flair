#include "main.hpp"
#include "particleController.hpp"

#include "UnityEngine/GameObject.hpp"

// Define a setter and getter function for a property that directly returns the value
#define DEFINE_BASIC_DIR_PROPERTY(moduleType, moduleName, propertyType, propertyName) \
propertyType Flare::ParticleController::moduleType::get_##propertyName(UnityEngine::ParticleSystem* particleSystem) { \
    static auto GetterFunc = il2cpp_utils::resolve_icall<propertyType, UnityEngine::ParticleSystem::moduleType*>("UnityEngine.ParticleSystem/" #moduleType "::get_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    return GetterFunc(&moduleObj); \
} \
void Flare::ParticleController::moduleType::set_##propertyName(UnityEngine::ParticleSystem* particleSystem, propertyType value) { \
    static auto SetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType>("UnityEngine.ParticleSystem/" #moduleType "::set_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    SetterFunc(&moduleObj, value); \
}

// Define a setter and getter function for a property that will only return the value by passing in a reference
#define DEFINE_BASIC_REF_PROPERTY(moduleType, moduleName, propertyType, propertyName) \
propertyType Flare::ParticleController::moduleType::get_##propertyName(UnityEngine::ParticleSystem *particleSystem) { \
    static auto GetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType*>("UnityEngine.ParticleSystem/" #moduleType "::get_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    propertyType propertyObj; \
    GetterFunc(&moduleObj, &propertyObj); \
    return propertyObj; \
} \
void Flare::ParticleController::moduleType::set_##propertyName(UnityEngine::ParticleSystem *particleSystem, propertyType value) { \
    static auto SetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType*>("UnityEngine.ParticleSystem/" #moduleType "::set_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    SetterFunc(&moduleObj, &value); \
}


DEFINE_BASIC_REF_PROPERTY(MainModule, main, UnityEngine::Vector3, emitterVelocity);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, duration);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, loop);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, prewarm);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startDelay);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startDelayMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startLifetime);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startLifetimeMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startSpeed);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startSpeedMultiplier);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, startSize3D);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startSize);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startSizeMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startSizeX);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startSizeXMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startSizeY);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startSizeYMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startSizeZ);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startSizeZMultiplier);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, startRotation3D);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startRotation);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startRotationMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startRotationX);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startRotationXMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startRotationY);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startRotationYMultiplier);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, startRotationZ);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, startRotationZMultiplier);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, flipRotation);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxGradient, startColor);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::GravitySource, gravitySource);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, Flare::ParticleController::MinMaxCurve, gravityModifier);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, gravityModifierMultiplier);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::SimulationSpace, simulationSpace);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, UnityEngine::Transform*, customSimulationSpace);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, float, simulationSpeed);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, useUnscaledTime);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::ScalingMode, scalingMode);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, bool, playOnAwake);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, int, maxParticles);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::EmitterVelocityMode, emitterVelocityMode);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::StopAction, stopAction);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::RingBufferMode, ringBufferMode);
DEFINE_BASIC_REF_PROPERTY(MainModule, main, UnityEngine::Vector2, ringBufferLoopRange);
DEFINE_BASIC_DIR_PROPERTY(MainModule, main, Flare::ParticleController::CullingMode, cullingMode);