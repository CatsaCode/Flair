#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"

#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Mesh.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/SkinnedMeshRenderer.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteRenderer.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/LayerMask.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Light.hpp"
#include "UnityEngine/AnimationCurve.hpp"
#include "UnityEngine/Gradient.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/ParticleSystem.hpp" // MinMaxCurve & MinMaxGradient (may recreate as a ParticleController struct)

namespace UnityEngine::Rendering {
    enum UVChannelFlags {UV0 = 1, UV1 = 2, UV2 = 4, UV3 = 8};
}

#define DECLARE_BASIC_PROPERTY(type, name) \
static type get_##name(UnityEngine::ParticleSystem* particleSystem); \
static void set_##name(UnityEngine::ParticleSystem* particleSystem, type value);

// DECLARE_CLASS_CODEGEN(Flare, ParticleControllerForceField, UnityEngine::Behaviour, 
//     public:
//         enum class ForceFieldShape {Sphere, Hemisphere, Cylinder, Box};

//         DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, rotationRandomness);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, directionX);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, directionY);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, directionZ);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, gravity);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, rotationSpeed);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, rotationAttraction);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, drag);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, vectorFieldSpeed);
//         DECLARE_BASIC_PROPERTY(MinMaxCurve, vectorFieldAttraction);
// );

// DECLARE_CLASS_CODEGEN(Flare, ParticleController, UnityEngine::MonoBehaviour,
namespace Flare {

    class ParticleController {
        public:
            enum class GravitySource {Physics3D, Physics2D};
            enum class SimulationSpace {Local, World, Custom};
            enum class ScalingMode {Hierarchy, Local, Shape};
            enum class EmitterVelocityMode {Transform, Rigidbody, Custom};
            enum class StopAction {None, Disable, Destroy, Callback};
            enum class RingBufferMode {Disabled, PauseUntilReplaced, LoopUntilReplaced};
            enum class CullingMode {Automatic, PauseAndCatchup, Pause, AlwaysSimulate};
            enum class ShapeType {Sphere, Hemisphere, Cone, Box, Mesh, ConeVolume, Circle, SingleSidedEdge, MeshRenderer, SkinnedMeshRenderer, BoxShell, BoxEdge, Donut, Rectangle, Sprite, SpriteRenderer};
            enum class ShapeMultiModeValue {Random, Loop, PingPong, BurstSpread};
            enum class MeshShapeType {Vertex, Edge, Triangle};
            enum class ShapeTextureChannel {Red, Green, Blue, Alpha};
            enum class CollisionType {Planes, World};
            enum class CollisionMode {Collision2D, Collision3D};
            enum class CollisionQuality {High, Medium, Low};
            enum class OverlapAction {Ignore, Kill, Callback};
            enum class ColliderQueryMode {Disabled, One, All};
            enum class SubEmitterType {Birth, Collision, Death, Trigger, Manual};
            enum class SubEmitterProperties {InheritNothing = 0, InheritEverything = 0x1f, InheritColor = 1, InheritSize = 2, InheritRotation = 4, InheritLifetime = 8, InheritDuration = 0x10};
            enum class AnimationMode {Grid, Sprites};
            enum class AnimationTimeMode {Lifetime, Speed, FPS};
            enum class AnimationType {WholeSheet, SingleRow};
            enum class AnimationRowMode {Custom, Random, MeshIndex};
            enum class InheritVelocityMode {Initial, Current};
            enum class GameObjectFilter {LayerMask, List, LayerMaskAndList};
            enum class NoiseQuality {Low, Medium, High};
            enum class TrailMode {PerParticle, Ribbon};
            enum class TrailTextureMode {Stretch, Tile, DistributePerSegment, RepeatPerSegment, Static};
            enum class CustomData {Custom1, Custom2};
            enum class CustomDataMode {Disabled, Vector, Color};
            enum class CurveMode {Constant, Curve, TwoCurves, TwoConstants};
            enum class GradientMode {Color, Gradient, TwoColors, TwoGradients, RandomColor};

            struct MinMaxCurve {
                CurveMode mode;
                float curveMultiplier;
                UnityEngine::AnimationCurve* curveMin = nullptr;
                UnityEngine::AnimationCurve* curveMax = nullptr;
                float constantMin;
                float constantMax;
            };

            struct MinMaxGradient {
                GradientMode mode;
                UnityEngine::Gradient* gradientMin;
                UnityEngine::Gradient* gradientMax;
                UnityEngine::Color colorMin;
                UnityEngine::Color colorMax;
            };

            struct Burst {
                float time;
                MinMaxCurve count;
                int repeatCount;
                float repeatInterval;
                float invProbability;
            };

            struct MainModule {
                DECLARE_BASIC_PROPERTY(UnityEngine::Vector3, emitterVelocity);
                DECLARE_BASIC_PROPERTY(float, duration);
                DECLARE_BASIC_PROPERTY(bool, loop);
                DECLARE_BASIC_PROPERTY(bool, prewarm);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startDelay);
                DECLARE_BASIC_PROPERTY(float, startDelayMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startLifetime);
                DECLARE_BASIC_PROPERTY(float, startLifetimeMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startSpeed);
                DECLARE_BASIC_PROPERTY(float, startSpeedMultiplier);
                DECLARE_BASIC_PROPERTY(bool, startSize3D);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startSize);
                DECLARE_BASIC_PROPERTY(float, startSizeMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startSizeX);
                DECLARE_BASIC_PROPERTY(float, startSizeXMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startSizeY);
                DECLARE_BASIC_PROPERTY(float, startSizeYMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startSizeZ);
                DECLARE_BASIC_PROPERTY(float, startSizeZMultiplier);
                DECLARE_BASIC_PROPERTY(bool, startRotation3D);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startRotation);
                DECLARE_BASIC_PROPERTY(float, startRotationMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startRotationX);
                DECLARE_BASIC_PROPERTY(float, startRotationXMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startRotationY);
                DECLARE_BASIC_PROPERTY(float, startRotationYMultiplier);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, startRotationZ);
                DECLARE_BASIC_PROPERTY(float, startRotationZMultiplier);
                DECLARE_BASIC_PROPERTY(float, flipRotation);
                DECLARE_BASIC_PROPERTY(MinMaxGradient, startColor);
                DECLARE_BASIC_PROPERTY(GravitySource, gravitySource);
                DECLARE_BASIC_PROPERTY(MinMaxCurve, gravityModifier);
                DECLARE_BASIC_PROPERTY(float, gravityModifierMultiplier);
                DECLARE_BASIC_PROPERTY(SimulationSpace, simulationSpace);
                DECLARE_BASIC_PROPERTY(UnityEngine::Transform*, customSimulationSpace);
                DECLARE_BASIC_PROPERTY(float, simulationSpeed);
                DECLARE_BASIC_PROPERTY(bool, useUnscaledTime);
                DECLARE_BASIC_PROPERTY(ScalingMode, scalingMode);
                DECLARE_BASIC_PROPERTY(bool, playOnAwake);
                DECLARE_BASIC_PROPERTY(int, maxParticles);
                DECLARE_BASIC_PROPERTY(EmitterVelocityMode, emitterVelocityMode);
                DECLARE_BASIC_PROPERTY(StopAction, stopAction);
                DECLARE_BASIC_PROPERTY(RingBufferMode, ringBufferMode);
                DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, ringBufferLoopRange);
                DECLARE_BASIC_PROPERTY(CullingMode, cullingMode);
            };

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, rateOverTime);
            //     DECLARE_BASIC_PROPERTY(float, rateOverTimeMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, rateOverDistance);
            //     DECLARE_BASIC_PROPERTY(float, rateOverDistanceMultiplier);
            //     Burst GetBurst(int index);
            //     void SetBurst(int index, Burst burst);
            //     DECLARE_BASIC_PROPERTY(int, burstCount);
            // } emission;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(ShapeType, shapeType);
            //     DECLARE_BASIC_PROPERTY(float, randomDirectionAmount);
            //     DECLARE_BASIC_PROPERTY(float, sphericalDirectionAmount);
            //     DECLARE_BASIC_PROPERTY(float, randomPositionAmount);
            //     DECLARE_BASIC_PROPERTY(bool, alignToDirection);
            //     DECLARE_BASIC_PROPERTY(float, radius);
            //     DECLARE_BASIC_PROPERTY(ShapeMultiModeValue, radiusMode);
            //     DECLARE_BASIC_PROPERTY(float, radiusSpread);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, radiusSpeed);
            //     DECLARE_BASIC_PROPERTY(float, radiusSpeedMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, radiusThickness);
            //     DECLARE_BASIC_PROPERTY(float, angle);
            //     DECLARE_BASIC_PROPERTY(float, length);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector3, boxThickness);
            //     DECLARE_BASIC_PROPERTY(MeshShapeType, meshShapeType);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Mesh, mesh);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::MeshRenderer, meshRenderer);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::SkinnedMeshRenderer, skinnedMeshRenderer);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Sprite, sprite);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::SpriteRenderer, spriteRenderer);
            //     DECLARE_BASIC_PROPERTY(bool, useMeshMaterialIndex);
            //     DECLARE_BASIC_PROPERTY(int, meshMaterialIndex);
            //     DECLARE_BASIC_PROPERTY(bool, useMeshColors);
            //     DECLARE_BASIC_PROPERTY(float, normalOffset);
            //     DECLARE_BASIC_PROPERTY(ShapeMultiModeValue, meshSpawnMode);
            //     DECLARE_BASIC_PROPERTY(float, meshSpawnSpread);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, meshSpawnSpeed);
            //     DECLARE_BASIC_PROPERTY(float, meshSpawnSpeedMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, arc);
            //     DECLARE_BASIC_PROPERTY(ShapeMultiModeValue, arcMode);
            //     DECLARE_BASIC_PROPERTY(float, arcSpread);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, arcSpeed);
            //     DECLARE_BASIC_PROPERTY(float, arcSpeedMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, donutRadius);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector3, position);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector3, rotation);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector3, scale);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Texture2D, texture);
            //     DECLARE_BASIC_PROPERTY(ShapeTextureChannel, textureClipChannel);
            //     DECLARE_BASIC_PROPERTY(float, textureClipThreshold);
            //     DECLARE_BASIC_PROPERTY(bool, textureColorAffectsParticles);
            //     DECLARE_BASIC_PROPERTY(bool, textureAlphaAffectsParticles);
            //     DECLARE_BASIC_PROPERTY(bool, textureBilinearFiltering);
            //     DECLARE_BASIC_PROPERTY(int, textureUVChannel);
            // } shape;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled)
            //     DECLARE_BASIC_PROPERTY(CollisionType, type)
            //     DECLARE_BASIC_PROPERTY(CollisionMode, mode)
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, dampen)
            //     DECLARE_BASIC_PROPERTY(float, dampenMultiplier)
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, bounce)
            //     DECLARE_BASIC_PROPERTY(float, bounceMultiplier)
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, lifetimeLoss)
            //     DECLARE_BASIC_PROPERTY(float, lifetimeLossMultiplier)
            //     DECLARE_BASIC_PROPERTY(float, minKillSpeed)
            //     DECLARE_BASIC_PROPERTY(float, maxKillSpeed)
            //     DECLARE_BASIC_PROPERTY(UnityEngine::LayerMask, collidesWith)
            //     DECLARE_BASIC_PROPERTY(bool, enableDynamicColliders)
            //     DECLARE_BASIC_PROPERTY(int, maxCollisionShapes)
            //     DECLARE_BASIC_PROPERTY(CollisionQuality, quality)
            //     DECLARE_BASIC_PROPERTY(float, voxelSize)
            //     DECLARE_BASIC_PROPERTY(float, radiusScale)
            //     DECLARE_BASIC_PROPERTY(bool, sendCollisionMessages)
            //     DECLARE_BASIC_PROPERTY(float, colliderForce)
            //     DECLARE_BASIC_PROPERTY(bool, multiplyColliderForceByCollisionAngle)
            //     DECLARE_BASIC_PROPERTY(bool, multiplyColliderForceByParticleSpeed)
            //     DECLARE_BASIC_PROPERTY(bool, multiplyColliderForceByParticleSize)
            //     void AddPlane(UnityEngine::Transform transform);
            //     void RemovePlane(int index);
            //     void RemovePlaneObject(UnityEngine::Transform transform);
            //     void SetPlane(int index, UnityEngine::Transform transform);
            //     UnityEngine::Transform GetPlane(int index);
            //     void GetPlaneCount();
            //     DECLARE_BASIC_PROPERTY(bool, enableInteriorCollisions)
            // } collision;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(OverlapAction, inside);
            //     DECLARE_BASIC_PROPERTY(OverlapAction, outside);
            //     DECLARE_BASIC_PROPERTY(OverlapAction, enter);
            //     DECLARE_BASIC_PROPERTY(OverlapAction, exit);
            //     DECLARE_BASIC_PROPERTY(ColliderQueryMode, colliderQueryMode);
            //     DECLARE_BASIC_PROPERTY(float, radiusScale);
            //     void AddCollider(UnityEngine::Component collider);
            //     void RemoveCollider(int index);
            //     void RemoveColliderObject(UnityEngine::Component component);
            //     void SetCollider(int index, UnityEngine::Component collider);
            //     UnityEngine::Component GetCollider(int index);
            //     void GetColliderCount();
            // } trigger;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     void GetSubEmittersCount();
            //     void AddSubEmitter(ParticleController subEmitter, SubEmitterType type, SubEmitterProperties properties, float emitProbability = 1.0f);
            //     void RemoveSubEmitter(int index);
            //     void RemoveSubEmitterObject(ParticleController subEmitter);
            //     void SetSubEmitterSystem(int index, ParticleController subEmitter);
            //     void SetSubEmitterType(int index, SubEmitterType type);
            //     void SetSubEmitterProperties(int index, SubEmitterProperties properties);
            //     void SetSubEmitterEmitProbability(int index, float emitProbability);
            //     ParticleController GetSubEmitterSystem(int index);
            //     SubEmitterType GetSubEmitterType(int index);
            //     SubEmitterProperties GetSubEmitterProperties(int index);
            //     float GetSubEmitterEmitProbability(int index);
            // } subEmitter;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(AnimationMode, mode);
            //     DECLARE_BASIC_PROPERTY(AnimationTimeMode, timeMode);
            //     DECLARE_BASIC_PROPERTY(float, fps);
            //     DECLARE_BASIC_PROPERTY(int, numTilesX);
            //     DECLARE_BASIC_PROPERTY(int, numTilesY);
            //     DECLARE_BASIC_PROPERTY(AnimationType, animation);
            //     DECLARE_BASIC_PROPERTY(AnimationRowMode, rowMode);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, frameOverTime);
            //     DECLARE_BASIC_PROPERTY(float, frameOverTimeMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, startFrame);
            //     DECLARE_BASIC_PROPERTY(float, startFrameMultiplier);
            //     DECLARE_BASIC_PROPERTY(int, cycleCount);
            //     DECLARE_BASIC_PROPERTY(int, rowIndex);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Rendering::UVChannelFlags, uvChannelMask);
            //     void GetSpriteCount();
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, speedRange);
            //     void AddSprite(UnityEngine::Sprite sprite);
            //     void RemoveSprite(int index);
            //     void SetSprite(int index, UnityEngine::Sprite sprite);
            //     UnityEngine::Sprite GetSprite(int index);
            // } textureSheetAnimation;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalX);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalY);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalZ);
            //     DECLARE_BASIC_PROPERTY(float, orbitalXMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, orbitalYMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, orbitalZMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalOffsetX);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalOffsetY);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, orbitalOffsetZ);
            //     DECLARE_BASIC_PROPERTY(float, orbitalOffsetXMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, orbitalOffsetYMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, orbitalOffsetZMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, radial);
            //     DECLARE_BASIC_PROPERTY(float, radialMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, speedModifier);
            //     DECLARE_BASIC_PROPERTY(float, speedModifierMultiplier);
            //     DECLARE_BASIC_PROPERTY(SimulationSpace, space);
            // } velocityOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, limitX);
            //     DECLARE_BASIC_PROPERTY(float, limitXMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, limitY);
            //     DECLARE_BASIC_PROPERTY(float, limitYMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, limitZ);
            //     DECLARE_BASIC_PROPERTY(float, limitZMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, limit);
            //     DECLARE_BASIC_PROPERTY(float, limitMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, dampen);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            //     DECLARE_BASIC_PROPERTY(SimulationSpace, space);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, drag);
            //     DECLARE_BASIC_PROPERTY(float, dragMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, multiplyDragByParticleSize);
            //     DECLARE_BASIC_PROPERTY(bool, multiplyDragByParticleVelocity);
            // } limitVelocityOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(InheritVelocityMode, mode);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, curve);
            //     DECLARE_BASIC_PROPERTY(float, curveMultiplier);
            // } inheritVelocity;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, curve);
            //     DECLARE_BASIC_PROPERTY(float, curveMultiplier);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, range);
            // } lifetimeByEmitterSpeed;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(SimulationSpace, space);
            //     DECLARE_BASIC_PROPERTY(bool, randomized);
            // } forceOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::ParticleSystem::MinMaxGradient, color);
            // } colorOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::ParticleSystem::MinMaxGradient, color);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, range);
            // } colorBySpeed;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, size);
            //     DECLARE_BASIC_PROPERTY(float, sizeMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            // } sizeOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, size);
            //     DECLARE_BASIC_PROPERTY(float, sizeMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, range);
            // } sizeBySpeed;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            // } rotationOverLifetime;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, x);
            //     DECLARE_BASIC_PROPERTY(float, xMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, y);
            //     DECLARE_BASIC_PROPERTY(float, yMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, z);
            //     DECLARE_BASIC_PROPERTY(float, zMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, range);
            // } rotationBySpeed;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(float, multiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, multiplierCurve);
            //     DECLARE_BASIC_PROPERTY(GameObjectFilter, influenceFilter);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::LayerMask, influenceMask);
            //     void GetInfluenceCount();
            //     bool IsAffectedBy(ParticleControllerForceField field);
            //     void AddInfluence(ParticleControllerForceField field);
            //     void RemoveInfluenceAtIndex(int index);
            //     void RemoveInfluence(ParticleControllerForceField field);
            //     void RemoveAllInfluences();
            //     void SetInfluence(int index, ParticleControllerForceField field);
            //     ParticleControllerForceField GetInfluence(int index);
            // } externalForces;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(bool, separateAxes);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, strength);
            //     DECLARE_BASIC_PROPERTY(float, strengthMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, strengthX);
            //     DECLARE_BASIC_PROPERTY(float, strengthXMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, strengthY);
            //     DECLARE_BASIC_PROPERTY(float, strengthYMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, strengthZ);
            //     DECLARE_BASIC_PROPERTY(float, strengthZMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, frequency);
            //     DECLARE_BASIC_PROPERTY(bool, damping);
            //     DECLARE_BASIC_PROPERTY(int, octaveCount);
            //     DECLARE_BASIC_PROPERTY(float, octaveMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, octaveScale);
            //     DECLARE_BASIC_PROPERTY(NoiseQuality, quality);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, scrollSpeed);
            //     DECLARE_BASIC_PROPERTY(float, scrollSpeedMultiplier);
            //     DECLARE_BASIC_PROPERTY(bool, remapEnabled);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, remap);
            //     DECLARE_BASIC_PROPERTY(float, remapMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, remapX);
            //     DECLARE_BASIC_PROPERTY(float, remapXMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, remapY);
            //     DECLARE_BASIC_PROPERTY(float, remapYMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, remapZ);
            //     DECLARE_BASIC_PROPERTY(float, remapZMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, positionAmount);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, rotationAmount);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, sizeAmount);
            // } noise;

            // struct {            
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(float, ratio);
            //     DECLARE_BASIC_PROPERTY(bool, useRandomDistribution);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Light, light);
            //     DECLARE_BASIC_PROPERTY(bool, useParticleColor);
            //     DECLARE_BASIC_PROPERTY(bool, sizeAffectsRange);
            //     DECLARE_BASIC_PROPERTY(bool, alphaAffectsIntensity);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, range);
            //     DECLARE_BASIC_PROPERTY(float, rangeMultiplier);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, intensity);
            //     DECLARE_BASIC_PROPERTY(float, intensityMultiplier);
            //     DECLARE_BASIC_PROPERTY(int, maxLights);
            // } lights;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     DECLARE_BASIC_PROPERTY(TrailMode, mode);
            //     DECLARE_BASIC_PROPERTY(float, ratio);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, lifetime);
            //     DECLARE_BASIC_PROPERTY(float, lifetimeMultiplier);
            //     DECLARE_BASIC_PROPERTY(float, minVertexDistance);
            //     DECLARE_BASIC_PROPERTY(TrailTextureMode, textureMode);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::Vector2, textureScale);
            //     DECLARE_BASIC_PROPERTY(bool, worldSpace);
            //     DECLARE_BASIC_PROPERTY(bool, dieWithParticles);
            //     DECLARE_BASIC_PROPERTY(bool, sizeAffectsWidth);
            //     DECLARE_BASIC_PROPERTY(bool, sizeAffectsLifetime);
            //     DECLARE_BASIC_PROPERTY(bool, inheritParticleColor);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::ParticleSystem::MinMaxGradient, colorOverLifetime);
            //     DECLARE_BASIC_PROPERTY(MinMaxCurve, widthOverTrail);
            //     DECLARE_BASIC_PROPERTY(float, widthOverTrailMultiplier);
            //     DECLARE_BASIC_PROPERTY(UnityEngine::ParticleSystem::MinMaxGradient, colorOverTrail);
            //     DECLARE_BASIC_PROPERTY(bool, generateLightingData);
            //     DECLARE_BASIC_PROPERTY(int, ribbonCount);
            //     DECLARE_BASIC_PROPERTY(float, shadowBias);
            //     DECLARE_BASIC_PROPERTY(bool, splitSubEmitterRibbons);
            //     DECLARE_BASIC_PROPERTY(bool, attachRibbonsToTransform);
            // } trails;

            // struct {
            //     DECLARE_BASIC_PROPERTY(bool, enabled);
            //     void SetMode(CustomData stream, CustomDataMode mode);
            //     CustomDataMode GetMode(CustomData stream);
            //     void SetVectorComponentCount(CustomData stream, int count);
            //     int GetVectorComponentCount(CustomData stream);
            //     void SetVector(CustomData stream, int component, MinMaxCurve curve);
            //     MinMaxCurve GetVector(CustomData stream, int component);
            //     void SetColor(CustomData stream, UnityEngine::ParticleSystem::MinMaxGradient gradient);
            //     UnityEngine::ParticleSystem::MinMaxGradient GetColor(CustomData stream);
            // } customData;

            // // Some fields and methods still need be created
    };

}