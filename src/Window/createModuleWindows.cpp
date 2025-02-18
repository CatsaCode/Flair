#include "Window/createModuleWindows.hpp"
#include "main.hpp"

#include "Window/createSettings.hpp"

#include "particleInterface.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace Flair::Window {

    Window* CreateMainModuleWindow(Window* parent, UnityEngine::ParticleSystem* particleSystem) {
        // Create base window
        Window* window = CreateWindow(parent, "Main Module", {60, -1});

        // Create a VerticalLayoutGroup that will automatically space the settings
        auto vertical = GameObject::New_ctor("Vertical")->AddComponent<UI::VerticalLayoutGroup*>();
        vertical->get_transform()->SetParent(window->body->get_transform(), false);
        vertical->set_childControlWidth(true); // Update width for all child objects
        vertical->set_childControlHeight(false); // Passively read hight from child object
        auto verticalFitter = vertical->get_gameObject()->AddComponent<ContentSizeFitter*>();
        verticalFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize); // Use the height from child objects

        // Create each setting
        CreateFloatSetting(vertical->get_transform(), "Duration", "The length of time the Particle System is emitting particles. If the system is looping, this indicates the length of one cycle.", ParticleInterface::MainModule::get_duration(particleSystem), [particleSystem](float value){ParticleInterface::MainModule::set_duration(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Loop", "If true, the emission cycle will repeat after the duration.", ParticleInterface::MainModule::get_loop(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_loop(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Prewarm", "When played, a prewarmed system will be in a state as if it had emitted one loop cycle. Can only be used if the system is looping.", ParticleInterface::MainModule::get_prewarm(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_prewarm(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start delay", "Delay in seconds that this Particle System will wait before emitting particles. Cannot be used together with a prewarmed looping system.", ParticleInterface::MainModule::get_startDelay(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startDelay(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start lifetime", "Start lifetime in seconds, particle will die when its lifetime reaches 0.", ParticleInterface::MainModule::get_startLifetime(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startLifetime(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start speed", "The start speed of particles, applied in the starting direction.", ParticleInterface::MainModule::get_startSpeed(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSpeed(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "3D Start size", "If enabled, you can control the size separately for each axis.", ParticleInterface::MainModule::get_startSize3D(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_startSize3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start size", "The start size of particles.", ParticleInterface::MainModule::get_startSize(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSize(particleSystem, value);});
        Create3DMinMaxCurveSetting(vertical->get_transform(), "", "", 
            ParticleInterface::MainModule::get_startSizeX(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeX(particleSystem, value);},
            ParticleInterface::MainModule::get_startSizeY(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeY(particleSystem, value);},
            ParticleInterface::MainModule::get_startSizeZ(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeZ(particleSystem, value);}
        );
        CreateBoolSetting(vertical->get_transform(), "3D Start rotation", "If enabled, you can control the rotation separately for each axis.", ParticleInterface::MainModule::get_startRotation3D(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_startRotation3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start rotation", "The start rotation of particles in degrees.", ParticleInterface::MainModule::get_startRotation(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotation(particleSystem, value);});
        Create3DMinMaxCurveSetting(vertical->get_transform(), "", "", 
            ParticleInterface::MainModule::get_startRotationX(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationX(particleSystem, value);},
            ParticleInterface::MainModule::get_startRotationY(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationY(particleSystem, value);},
            ParticleInterface::MainModule::get_startRotationZ(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationZ(particleSystem, value);}
        );
        CreateFloatSetting(vertical->get_transform(), "Flip rotation", "Cause some particles to spin in the opposite direction. (Set between 0 and 1, where a higher value causes more to flip)", ParticleInterface::MainModule::get_flipRotation(particleSystem), [particleSystem](float value){ParticleInterface::MainModule::set_flipRotation(particleSystem, value);});
        CreateMinMaxGradientSetting(vertical->get_transform(), "Start color", "The start color of particles.", ParticleInterface::MainModule::get_startColor(particleSystem), [particleSystem](ParticleInterface::MinMaxGradient value){ParticleInterface::MainModule::set_startColor(particleSystem, value);});
        CreateDropdownSetting<ParticleInterface::SimulationSpace>(vertical->get_transform(), "Simulation space", "Makes particle positions simulate in world, local, or custom space. In local space they stay relative to their own Transform, and in custom space they are relative to the custom Transform.", {"Local", "World", "Custom"}, {ParticleInterface::SimulationSpace::Local, ParticleInterface::SimulationSpace::World, ParticleInterface::SimulationSpace::Custom}, ParticleInterface::MainModule::get_simulationSpace(particleSystem), [particleSystem](ParticleInterface::SimulationSpace value){ParticleInterface::MainModule::set_simulationSpace(particleSystem, value);});
        // TODO CreateTransformSetting("Custom simulation space", ...);
        CreateFloatSetting(vertical->get_transform(), "Simulation speed", "Scale the playback speed of the Particle System.", ParticleInterface::MainModule::get_simulationSpeed(particleSystem), [particleSystem](float value){ParticleInterface::MainModule::set_simulationSpeed(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Unscaled delta time", "Use Unscaled Delta Time instead of Delta Time. Useful for playing effects whilst paused.", ParticleInterface::MainModule::get_useUnscaledTime(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_useUnscaledTime(particleSystem, value);});
        CreateDropdownSetting<ParticleInterface::ScalingMode>(vertical->get_transform(), "Scaling mode", "Use the combined scale from our entire hierarchy, just this local particle mode, or only apply scale to the shape module.", {"Hierarchy", "Local", "Shape"}, {ParticleInterface::ScalingMode::Hierarchy, ParticleInterface::ScalingMode::Local, ParticleInterface::ScalingMode::Shape}, ParticleInterface::MainModule::get_scalingMode(particleSystem), [particleSystem](ParticleInterface::ScalingMode value){ParticleInterface::MainModule::set_scalingMode(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Play on awake", "If enabled, the system will start playing automatically. Note that this setting is shared between all Particle Systems in the current particle effect.", ParticleInterface::MainModule::get_playOnAwake(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_playOnAwake(particleSystem, value);});
        CreateDropdownSetting<ParticleInterface::EmitterVelocityMode>(vertical->get_transform(), "Emitter velocity mode", "How to determine the extra velocity imparted to particles by the Particle System. Use the Transform option to add the velocity calculated from the system's Transform. Use Rigidbody to add the velocity of the system's Rigidbody. Use Custom to add the velocity specified in the Custom Velocity property.", {"Transform", "Rigidbody", "Custom"}, {ParticleInterface::EmitterVelocityMode::Transform, ParticleInterface::EmitterVelocityMode::Rigidbody, ParticleInterface::EmitterVelocityMode::Custom}, ParticleInterface::MainModule::get_emitterVelocityMode(particleSystem), [particleSystem](ParticleInterface::EmitterVelocityMode value){ParticleInterface::MainModule::set_emitterVelocityMode(particleSystem, value);});
        // TODO CreateVector3Setting("Custom velocity", ...);
        CreateIntSetting(vertical->get_transform(), "Max particles", "The number of particles in the system will be limited by this number. Emission will be temporarily halted if this is reached.", ParticleInterface::MainModule::get_maxParticles(particleSystem), [particleSystem](int value){ParticleInterface::MainModule::set_maxParticles(particleSystem, value);});
        // TODO Find where random seed is even defined
        // TODO DANGER!!! Don't let objects destroy themselves!!!
        CreateDropdownSetting<ParticleInterface::StopAction>(vertical->get_transform(), "Stop action", "When the Particle System is stopped and all particles have died, should the GameObject automatically disable/destroy itself?", {"None", "Disable", "Destroy", "Callback"}, {ParticleInterface::StopAction::None, ParticleInterface::StopAction::Disable, ParticleInterface::StopAction::Destroy, ParticleInterface::StopAction::Callback}, ParticleInterface::MainModule::get_stopAction(particleSystem), [particleSystem](ParticleInterface::StopAction value){ParticleInterface::MainModule::set_stopAction(particleSystem, value);});
        CreateDropdownSetting<ParticleInterface::CullingMode>(vertical->get_transform(), "Culling mode", "Choose whether to continue simulating the Particle System when offscreen. Catch-up mode pauses offscreen simulations but performs a large simulation step when they become visible, giving the appearance that they were never paused. Automatic uses Pause mode for looping systems, and AlwaysSimulate if not looping.", {"Automatic", "Pause and Catch-up", "Pause", "Always Simulate"}, {ParticleInterface::CullingMode::Automatic, ParticleInterface::CullingMode::PauseAndCatchup, ParticleInterface::CullingMode::Pause, ParticleInterface::CullingMode::AlwaysSimulate}, ParticleInterface::MainModule::get_cullingMode(particleSystem), [particleSystem](ParticleInterface::CullingMode value){ParticleInterface::MainModule::set_cullingMode(particleSystem, value);});
        CreateDropdownSetting<ParticleInterface::RingBufferMode>(vertical->get_transform(), "Ring buffer mode", "Rather than dying when their lifetime has elapsed, particles will remain alive until the Max Particles buffer is full, at which point new particles will replace the oldest.", {"Disabled", "Pause Until Replaced", "Loop Until Replaced"}, {ParticleInterface::RingBufferMode::Disabled, ParticleInterface::RingBufferMode::PauseUntilReplaced, ParticleInterface::RingBufferMode::LoopUntilReplaced}, ParticleInterface::MainModule::get_ringBufferMode(particleSystem), [particleSystem](ParticleInterface::RingBufferMode value){ParticleInterface::MainModule::set_ringBufferMode(particleSystem, value);});
        // TODO CreateVector2Setting("Loop range", ...);
        
        // Update size of the window
        window->SetDirty(true);
        window->SetDirty(1.0f);

        return window;
    }

}