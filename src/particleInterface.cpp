#include "main.hpp"
#include "particleInterface.hpp"

#include "UnityEngine/GameObject.hpp"

using namespace Flair;

// Define a setter and getter function for a property that directly returns the value
#define DEFINE_FIELD_SETGET_DIRECT(moduleType, moduleName, propertyType, propertyName) \
propertyType ParticleInterface::moduleType::get_##propertyName(UnityEngine::ParticleSystem* particleSystem) { \
    static auto GetterFunc = il2cpp_utils::resolve_icall<propertyType, UnityEngine::ParticleSystem::moduleType*>("UnityEngine.ParticleSystem/" #moduleType "::get_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    return GetterFunc(&moduleObj); \
} \
void ParticleInterface::moduleType::set_##propertyName(UnityEngine::ParticleSystem* particleSystem, propertyType value) { \
    static auto SetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType>("UnityEngine.ParticleSystem/" #moduleType "::set_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    SetterFunc(&moduleObj, value); \
}

// Define a setter and getter function for a property that will only return the value by passing in a reference
#define DEFINE_FIELD_SETGET_BY_REF(moduleType, moduleName, propertyType, propertyName) \
propertyType ParticleInterface::moduleType::get_##propertyName(UnityEngine::ParticleSystem *particleSystem) { \
    static auto GetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType*>("UnityEngine.ParticleSystem/" #moduleType "::get_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    propertyType propertyObj; \
    GetterFunc(&moduleObj, &propertyObj); \
    return propertyObj; \
} \
void ParticleInterface::moduleType::set_##propertyName(UnityEngine::ParticleSystem *particleSystem, propertyType value) { \
    static auto SetterFunc = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, propertyType*>("UnityEngine.ParticleSystem/" #moduleType "::set_" #propertyName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    SetterFunc(&moduleObj, &value); \
}

#define DEFINE_VOID_1(moduleType, moduleName, funcName, para1Type, para1Name) \
void ParticleInterface::moduleType::funcName(UnityEngine::ParticleSystem* particleSystem, para1Type para1Name) { \
    static auto Func = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, para1Type>("UnityEngine.ParticleSystem/" #moduleType "::" #funcName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    Func(&moduleObj, para1Name); \
}

#define DEFINE_VOID_2(moduleType, moduleName, funcName, para1Type, para1Name, para2Type, para2Name) \
void ParticleInterface::moduleType::funcName(UnityEngine::ParticleSystem* particleSystem, para1Type para1Name, para2Type para2Name) { \
    static auto Func = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, para1Type, para2Type>("UnityEngine.ParticleSystem/" #moduleType "::" #funcName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    Func(&moduleObj, para1Name, para2Name); \
}

#define DEFINE_VOID_4(moduleType, moduleName, funcName, para1Type, para1Name, para2Type, para2Name, para3Type, para3Name, para4Type, para4Name) \
void ParticleInterface::moduleType::funcName(UnityEngine::ParticleSystem* particleSystem, para1Type para1Name, para2Type para2Name, para3Type para3Name, para4Type para4Name) { \
    static auto Func = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::moduleType*, para1Type, para2Type, para3Type, para4Type>("UnityEngine.ParticleSystem/" #moduleType "::" #funcName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    Func(&moduleObj, para1Name, para2Name, para3Name, para4Name); \
}

#define DEFINE_RETURN_0(returnType, moduleType, moduleName, funcName) \
returnType ParticleInterface::moduleType::funcName(UnityEngine::ParticleSystem* particleSystem) { \
    static auto Func = il2cpp_utils::resolve_icall<returnType, UnityEngine::ParticleSystem::moduleType*>("UnityEngine.ParticleSystem/" #moduleType "::" #funcName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    return Func(&moduleObj); \
}

#define DEFINE_RETURN_1(returnType, moduleType, moduleName, funcName, para1Type, para1Name) \
returnType ParticleInterface::moduleType::funcName(UnityEngine::ParticleSystem* particleSystem, para1Type para1Name) { \
    static auto Func = il2cpp_utils::resolve_icall<returnType, UnityEngine::ParticleSystem::moduleType*, para1Type>("UnityEngine.ParticleSystem/" #moduleType "::" #funcName "_Injected"); \
    UnityEngine::ParticleSystem::moduleType moduleObj = particleSystem->moduleName; \
    return Func(&moduleObj, para1Name); \
}


DEFINE_FIELD_SETGET_BY_REF(MainModule, main, UnityEngine::Vector3, emitterVelocity);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, duration);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, loop);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, prewarm);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startDelay);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startDelayMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startLifetime);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startLifetimeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startSpeed);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startSpeedMultiplier);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, startSize3D);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startSize);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startSizeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startSizeX);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startSizeXMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startSizeY);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startSizeYMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startSizeZ);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startSizeZMultiplier);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, startRotation3D);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startRotation);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startRotationMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startRotationX);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startRotationXMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startRotationY);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startRotationYMultiplier);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, startRotationZ);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, startRotationZMultiplier);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, flipRotation);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxGradient, startColor);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::GravitySource, gravitySource);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, ParticleInterface::MinMaxCurve, gravityModifier);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, gravityModifierMultiplier);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::SimulationSpace, simulationSpace);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, UnityEngine::Transform*, customSimulationSpace);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, float, simulationSpeed);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, useUnscaledTime);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::ScalingMode, scalingMode);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, bool, playOnAwake);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, int, maxParticles);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::EmitterVelocityMode, emitterVelocityMode);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::StopAction, stopAction);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::RingBufferMode, ringBufferMode);
DEFINE_FIELD_SETGET_BY_REF(MainModule, main, UnityEngine::Vector2, ringBufferLoopRange);
DEFINE_FIELD_SETGET_DIRECT(MainModule, main, ParticleInterface::CullingMode, cullingMode);

DEFINE_FIELD_SETGET_DIRECT(EmissionModule, emission, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(EmissionModule, emission, ParticleInterface::MinMaxCurve, rateOverTime);
DEFINE_FIELD_SETGET_DIRECT(EmissionModule, emission, float, rateOverTimeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(EmissionModule, emission, ParticleInterface::MinMaxCurve, rateOverDistance);
DEFINE_FIELD_SETGET_DIRECT(EmissionModule, emission, float, rateOverDistanceMultiplier);
ParticleInterface::Burst GetBurst(UnityEngine::ParticleSystem* particleSystem, int index) {
    static auto Func = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::EmissionModule*, int, ParticleInterface::Burst*>("UnityEngine.ParticleSystem/EmissionModule::GetBurst_Injected");
    UnityEngine::ParticleSystem::EmissionModule moduleObj = particleSystem->emission;
    ParticleInterface::Burst propertyObj;
    Func(&moduleObj, index, &propertyObj);
    return propertyObj;
}
void SetBurst(UnityEngine::ParticleSystem* particleSystem, int index, ParticleInterface::Burst burst) {
    static auto Func = il2cpp_utils::resolve_icall<void, UnityEngine::ParticleSystem::EmissionModule*, int, ParticleInterface::Burst*>("UnityEngine.ParticleSystem/EmissionModule::SetBurst_Injected");
    UnityEngine::ParticleSystem::EmissionModule moduleObj = particleSystem->emission;
    Func(&moduleObj, index, &burst);
}
DEFINE_FIELD_SETGET_DIRECT(EmissionModule, emission, int, burstCount);

DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::ShapeType, shapeType);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, randomDirectionAmount);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, sphericalDirectionAmount);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, randomPositionAmount);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, alignToDirection);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, radius);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::ShapeMultiModeValue, radiusMode);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, radiusSpread);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, ParticleInterface::MinMaxCurve, radiusSpeed);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, radiusSpeedMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, radiusThickness);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, angle);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, length);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, UnityEngine::Vector3, boxThickness);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::MeshShapeType, meshShapeType);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::Mesh*, mesh);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::MeshRenderer*, meshRenderer);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::SkinnedMeshRenderer*, skinnedMeshRenderer);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::Sprite*, sprite);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::SpriteRenderer*, spriteRenderer);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, useMeshMaterialIndex);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, int, meshMaterialIndex);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, useMeshColors);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, normalOffset);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::ShapeMultiModeValue, meshSpawnMode);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, meshSpawnSpread);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, ParticleInterface::MinMaxCurve, meshSpawnSpeed);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, meshSpawnSpeedMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, arc);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::ShapeMultiModeValue, arcMode);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, arcSpread);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, ParticleInterface::MinMaxCurve, arcSpeed);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, arcSpeedMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, donutRadius);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, UnityEngine::Vector3, position);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, UnityEngine::Vector3, rotation);
DEFINE_FIELD_SETGET_BY_REF(ShapeModule, shape, UnityEngine::Vector3, scale);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, UnityEngine::Texture2D*, texture);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, ParticleInterface::ShapeTextureChannel, textureClipChannel);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, float, textureClipThreshold);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, textureColorAffectsParticles);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, textureAlphaAffectsParticles);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, bool, textureBilinearFiltering);
DEFINE_FIELD_SETGET_DIRECT(ShapeModule, shape, int, textureUVChannel);

DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, ParticleInterface::CollisionType, type);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, ParticleInterface::CollisionMode, mode);
DEFINE_FIELD_SETGET_BY_REF(CollisionModule, collision, ParticleInterface::MinMaxCurve, dampen);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, dampenMultiplier);
DEFINE_FIELD_SETGET_BY_REF(CollisionModule, collision, ParticleInterface::MinMaxCurve, bounce);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, bounceMultiplier);
DEFINE_FIELD_SETGET_BY_REF(CollisionModule, collision, ParticleInterface::MinMaxCurve, lifetimeLoss);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, lifetimeLossMultiplier);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, minKillSpeed);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, maxKillSpeed);
DEFINE_FIELD_SETGET_BY_REF(CollisionModule, collision, UnityEngine::LayerMask, collidesWith);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, enableDynamicColliders);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, int, maxCollisionShapes);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, ParticleInterface::CollisionQuality, quality);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, voxelSize);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, radiusScale);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, sendCollisionMessages);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, float, colliderForce);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, multiplyColliderForceByCollisionAngle);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, multiplyColliderForceByParticleSpeed);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, multiplyColliderForceByParticleSize);
DEFINE_VOID_1(CollisionModule, collision, AddPlane, UnityEngine::Transform*, transform);
DEFINE_VOID_1(CollisionModule, collision, RemovePlane, int, index);
DEFINE_VOID_1(CollisionModule, collision, RemovePlaneObject, UnityEngine::Transform*, transform);
DEFINE_VOID_2(CollisionModule, collision, SetPlane, int, index, UnityEngine::Transform*, transform);
DEFINE_RETURN_1(UnityEngine::Transform*, CollisionModule, collision, GetPlane, int, index);
DEFINE_RETURN_0(int, CollisionModule, collision, get_planeCount);
DEFINE_FIELD_SETGET_DIRECT(CollisionModule, collision, bool, enableInteriorCollisions);

DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, ParticleInterface::OverlapAction, inside);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, ParticleInterface::OverlapAction, outside);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, ParticleInterface::OverlapAction, enter);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, ParticleInterface::OverlapAction, exit);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, ParticleInterface::ColliderQueryMode, colliderQueryMode);
DEFINE_FIELD_SETGET_DIRECT(TriggerModule, trigger, float, radiusScale);
DEFINE_VOID_1(TriggerModule, trigger, AddCollider, UnityEngine::Component*, collider);
DEFINE_VOID_1(TriggerModule, trigger, RemoveCollider, int, index);
DEFINE_VOID_1(TriggerModule, trigger, RemoveColliderObject, UnityEngine::Component*, component);
DEFINE_VOID_2(TriggerModule, trigger, SetCollider, int, index, UnityEngine::Component*, collider);
DEFINE_RETURN_1(UnityEngine::Component*, TriggerModule, trigger, GetCollider, int, index);
DEFINE_RETURN_0(int, TriggerModule, trigger, get_colliderCount);

DEFINE_FIELD_SETGET_DIRECT(SubEmittersModule, subEmitters, bool, enabled);
DEFINE_RETURN_0(int, SubEmittersModule, subEmitters, get_subEmittersCount);
DEFINE_VOID_4(SubEmittersModule, subEmitters, AddSubEmitter, UnityEngine::ParticleSystem*, subEmitter, SubEmitterType, type, SubEmitterProperties, properties, float, emitProbability);
DEFINE_VOID_1(SubEmittersModule, subEmitters, RemoveSubEmitter, int, index);
DEFINE_VOID_1(SubEmittersModule, subEmitters, RemoveSubEmitterObject, UnityEngine::ParticleSystem*, subEmitter);
DEFINE_VOID_2(SubEmittersModule, subEmitters, SetSubEmitterSystem, int, index, UnityEngine::ParticleSystem*, subEmitter);
DEFINE_VOID_2(SubEmittersModule, subEmitters, SetSubEmitterType, int, index, SubEmitterType, type);
DEFINE_VOID_2(SubEmittersModule, subEmitters, SetSubEmitterProperties, int, index, SubEmitterProperties, properties);
DEFINE_VOID_2(SubEmittersModule, subEmitters, SetSubEmitterEmitProbability, int, index, float, emitProbability);
DEFINE_RETURN_1(UnityEngine::ParticleSystem*, SubEmittersModule, subEmitters, GetSubEmitterSystem, int, index);
DEFINE_RETURN_1(ParticleInterface::SubEmitterType, SubEmittersModule, subEmitters, GetSubEmitterType, int, index);
DEFINE_RETURN_1(ParticleInterface::SubEmitterProperties, SubEmittersModule, subEmitters, GetSubEmitterProperties, int, index);
DEFINE_RETURN_1(float, SubEmittersModule, subEmitters, GetSubEmitterEmitProbability, int, index);

DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::AnimationMode, mode);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::AnimationTimeMode, timeMode);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, float, fps);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, int, numTilesX);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, int, numTilesY);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::AnimationType, animation);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::AnimationRowMode, rowMode);
DEFINE_FIELD_SETGET_BY_REF(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::MinMaxCurve, frameOverTime);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, float, frameOverTimeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(TextureSheetAnimationModule, textureSheetAnimation, ParticleInterface::MinMaxCurve, startFrame);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, float, startFrameMultiplier);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, int, cycleCount);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, int, rowIndex);
DEFINE_FIELD_SETGET_DIRECT(TextureSheetAnimationModule, textureSheetAnimation, UnityEngine::Rendering::UVChannelFlags, uvChannelMask);
DEFINE_RETURN_0(int, TextureSheetAnimationModule, textureSheetAnimation, get_spriteCount);
DEFINE_FIELD_SETGET_BY_REF(TextureSheetAnimationModule, textureSheetAnimation, UnityEngine::Vector2, speedRange);
DEFINE_VOID_1(TextureSheetAnimationModule, textureSheetAnimation, AddSprite, UnityEngine::Sprite*, sprite);
DEFINE_VOID_1(TextureSheetAnimationModule, textureSheetAnimation, RemoveSprite, int, index);
DEFINE_VOID_2(TextureSheetAnimationModule, textureSheetAnimation, SetSprite, int, index, UnityEngine::Sprite*, sprite);
DEFINE_RETURN_1(UnityEngine::Sprite*, TextureSheetAnimationModule, textureSheetAnimation, GetSprite, int, index);

DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, xMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, yMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, zMultiplier);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalX);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalY);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalZ);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalXMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalYMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalZMultiplier);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalOffsetX);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalOffsetY);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, orbitalOffsetZ);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalOffsetXMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalOffsetYMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, orbitalOffsetZMultiplier);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, radial);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, radialMultiplier);
DEFINE_FIELD_SETGET_BY_REF(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::MinMaxCurve, speedModifier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, float, speedModifierMultiplier);
DEFINE_FIELD_SETGET_DIRECT(VelocityOverLifetimeModule, velocityOverLifetime, ParticleInterface::SimulationSpace, space);

DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::MinMaxCurve, limitX);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, limitXMultiplier);
DEFINE_FIELD_SETGET_BY_REF(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::MinMaxCurve, limitY);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, limitYMultiplier);
DEFINE_FIELD_SETGET_BY_REF(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::MinMaxCurve, limitZ);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, limitZMultiplier);
DEFINE_FIELD_SETGET_BY_REF(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::MinMaxCurve, limit);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, limitMultiplier);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, dampen);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, bool, separateAxes);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::SimulationSpace, space);
DEFINE_FIELD_SETGET_BY_REF(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, ParticleInterface::MinMaxCurve, drag);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, float, dragMultiplier);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, bool, multiplyDragByParticleSize);
DEFINE_FIELD_SETGET_DIRECT(LimitVelocityOverLifetimeModule, limitVelocityOverLifetime, bool, multiplyDragByParticleVelocity);

DEFINE_FIELD_SETGET_DIRECT(InheritVelocityModule, inheritVelocity, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(InheritVelocityModule, inheritVelocity, ParticleInterface::InheritVelocityMode, mode);
DEFINE_FIELD_SETGET_BY_REF(InheritVelocityModule, inheritVelocity, ParticleInterface::MinMaxCurve, curve);
DEFINE_FIELD_SETGET_DIRECT(InheritVelocityModule, inheritVelocity, float, curveMultiplier);

DEFINE_FIELD_SETGET_DIRECT(LifetimeByEmitterSpeedModule, lifetimeByEmitterSpeed, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(LifetimeByEmitterSpeedModule, lifetimeByEmitterSpeed, ParticleInterface::MinMaxCurve, curve);
DEFINE_FIELD_SETGET_DIRECT(LifetimeByEmitterSpeedModule, lifetimeByEmitterSpeed, float, curveMultiplier);
DEFINE_FIELD_SETGET_BY_REF(LifetimeByEmitterSpeedModule, lifetimeByEmitterSpeed, UnityEngine::Vector2, range);

DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(ForceOverLifetimeModule, forceOverLifetime, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_BY_REF(ForceOverLifetimeModule, forceOverLifetime, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_BY_REF(ForceOverLifetimeModule, forceOverLifetime, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, float, xMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, float, yMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, float, zMultiplier);
DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, ParticleInterface::SimulationSpace, space);
DEFINE_FIELD_SETGET_DIRECT(ForceOverLifetimeModule, forceOverLifetime, bool, randomized);

DEFINE_FIELD_SETGET_DIRECT(ColorOverLifetimeModule, colorOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(ColorOverLifetimeModule, colorOverLifetime, ParticleInterface::MinMaxGradient, color);

DEFINE_FIELD_SETGET_DIRECT(ColorBySpeedModule, colorBySpeed, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(ColorBySpeedModule, colorBySpeed, ParticleInterface::MinMaxGradient, color);
DEFINE_FIELD_SETGET_BY_REF(ColorBySpeedModule, colorBySpeed, UnityEngine::Vector2, range);

DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(SizeOverLifetimeModule, sizeOverLifetime, ParticleInterface::MinMaxCurve, size);
DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, float, sizeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeOverLifetimeModule, sizeOverLifetime, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, float, xMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeOverLifetimeModule, sizeOverLifetime, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, float, yMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeOverLifetimeModule, sizeOverLifetime, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, float, zMultiplier);
DEFINE_FIELD_SETGET_DIRECT(SizeOverLifetimeModule, sizeOverLifetime, bool, separateAxes);

DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(SizeBySpeedModule, sizeBySpeed, ParticleInterface::MinMaxCurve, size);
DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, float, sizeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeBySpeedModule, sizeBySpeed, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, float, xMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeBySpeedModule, sizeBySpeed, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, float, yMultiplier);
DEFINE_FIELD_SETGET_BY_REF(SizeBySpeedModule, sizeBySpeed, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, float, zMultiplier);
DEFINE_FIELD_SETGET_DIRECT(SizeBySpeedModule, sizeBySpeed, bool, separateAxes);
DEFINE_FIELD_SETGET_BY_REF(SizeBySpeedModule, sizeBySpeed, UnityEngine::Vector2, range);

DEFINE_FIELD_SETGET_DIRECT(RotationOverLifetimeModule, rotationOverLifetime, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(RotationOverLifetimeModule, rotationOverLifetime, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_DIRECT(RotationOverLifetimeModule, rotationOverLifetime, float, xMultiplier);
DEFINE_FIELD_SETGET_BY_REF(RotationOverLifetimeModule, rotationOverLifetime, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_DIRECT(RotationOverLifetimeModule, rotationOverLifetime, float, yMultiplier);
DEFINE_FIELD_SETGET_BY_REF(RotationOverLifetimeModule, rotationOverLifetime, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(RotationOverLifetimeModule, rotationOverLifetime, float, zMultiplier);
DEFINE_FIELD_SETGET_DIRECT(RotationOverLifetimeModule, rotationOverLifetime, bool, separateAxes);

DEFINE_FIELD_SETGET_DIRECT(RotationBySpeedModule, rotationBySpeed, bool, enabled);
DEFINE_FIELD_SETGET_BY_REF(RotationBySpeedModule, rotationBySpeed, ParticleInterface::MinMaxCurve, x);
DEFINE_FIELD_SETGET_DIRECT(RotationBySpeedModule, rotationBySpeed, float, xMultiplier);
DEFINE_FIELD_SETGET_BY_REF(RotationBySpeedModule, rotationBySpeed, ParticleInterface::MinMaxCurve, y);
DEFINE_FIELD_SETGET_DIRECT(RotationBySpeedModule, rotationBySpeed, float, yMultiplier);
DEFINE_FIELD_SETGET_BY_REF(RotationBySpeedModule, rotationBySpeed, ParticleInterface::MinMaxCurve, z);
DEFINE_FIELD_SETGET_DIRECT(RotationBySpeedModule, rotationBySpeed, float, zMultiplier);
DEFINE_FIELD_SETGET_DIRECT(RotationBySpeedModule, rotationBySpeed, bool, separateAxes);
DEFINE_FIELD_SETGET_BY_REF(RotationBySpeedModule, rotationBySpeed, UnityEngine::Vector2, range);

// struct ExternalForcesModule
// DECLARE_BASIC_PROPERTY(bool, enabled);
// DECLARE_BASIC_PROPERTY(float, multiplier);
// DECLARE_BASIC_PROPERTY(MinMaxCurve, multiplierCurve);
// DECLARE_BASIC_PROPERTY(GameObjectFilter, influenceFilter);
// DECLARE_BASIC_PROPERTY(UnityEngine::LayerMask, influenceMask);
// void GetInfluenceCount();
// bool IsAffectedBy(ParticleInterfaceForceField field);
// void AddInfluence(ParticleInterfaceForceField field);
// void RemoveInfluenceAtIndex(int index);
// void RemoveInfluence(ParticleInterfaceForceField field);
// void RemoveAllInfluences();
// void SetInfluence(int index, ParticleInterfaceForceField field);
// ParticleInterfaceForceField GetInfluence(int index);

DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, bool, separateAxes);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, strength);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, strengthMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, strengthX);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, strengthXMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, strengthY);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, strengthYMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, strengthZ);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, strengthZMultiplier);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, frequency);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, bool, damping);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, int, octaveCount);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, octaveMultiplier);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, octaveScale);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, ParticleInterface::NoiseQuality, quality);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, scrollSpeed);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, scrollSpeedMultiplier);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, bool, remapEnabled);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, remap);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, remapMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, remapX);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, remapXMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, remapY);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, remapYMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, remapZ);
DEFINE_FIELD_SETGET_DIRECT(NoiseModule, noise, float, remapZMultiplier);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, positionAmount);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, rotationAmount);
DEFINE_FIELD_SETGET_BY_REF(NoiseModule, noise, ParticleInterface::MinMaxCurve, sizeAmount);

DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, float, ratio);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, bool, useRandomDistribution);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, UnityEngine::Light*, light);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, bool, useParticleColor);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, bool, sizeAffectsRange);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, bool, alphaAffectsIntensity);
DEFINE_FIELD_SETGET_BY_REF(LightsModule, lights, ParticleInterface::MinMaxCurve, range);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, float, rangeMultiplier);
DEFINE_FIELD_SETGET_BY_REF(LightsModule, lights, ParticleInterface::MinMaxCurve, intensity);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, float, intensityMultiplier);
DEFINE_FIELD_SETGET_DIRECT(LightsModule, lights, int, maxLights);

DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, enabled);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, ParticleInterface::TrailMode, mode);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, float, ratio);
DEFINE_FIELD_SETGET_BY_REF(TrailModule, trails, ParticleInterface::MinMaxCurve, lifetime);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, float, lifetimeMultiplier);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, float, minVertexDistance);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, ParticleInterface::TrailTextureMode, textureMode);
DEFINE_FIELD_SETGET_BY_REF(TrailModule, trails, UnityEngine::Vector2, textureScale);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, worldSpace);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, dieWithParticles);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, sizeAffectsWidth);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, sizeAffectsLifetime);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, inheritParticleColor);
DEFINE_FIELD_SETGET_BY_REF(TrailModule, trails, ParticleInterface::MinMaxGradient, colorOverLifetime);
DEFINE_FIELD_SETGET_BY_REF(TrailModule, trails, ParticleInterface::MinMaxCurve, widthOverTrail);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, float, widthOverTrailMultiplier);
DEFINE_FIELD_SETGET_BY_REF(TrailModule, trails, ParticleInterface::MinMaxGradient, colorOverTrail);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, generateLightingData);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, int, ribbonCount);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, float, shadowBias);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, splitSubEmitterRibbons);
DEFINE_FIELD_SETGET_DIRECT(TrailModule, trails, bool, attachRibbonsToTransform);

#undef DEFINE_FIELD_SETGET_DIRECT
#undef DEFINE_FIELD_SETGET_BY_REF
#undef DEFINE_VOID_1
#undef DEFINE_VOID_2
#undef DEFINE_VOID_4
#undef DEFINE_RETURN_0
#undef DEFINE_RETURN_1