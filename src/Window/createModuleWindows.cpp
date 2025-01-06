#include "Window/createModuleWindows.hpp"
#include "main.hpp"

#include "Window/createSettings.hpp"

#include "particleController.hpp"

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace Flair::Window {

    Window* CreateMainModuleWindow(Window* parent, UnityEngine::ParticleSystem* particleSystem) {
        // Create base window
        Window* window = CreateWindow(parent, "Main Module", {50, -1});

        // Create a VerticalLayoutGroup that will automatically space the settings
        auto vertical = GameObject::New_ctor("Vertical")->AddComponent<UI::VerticalLayoutGroup*>();
        vertical->get_transform()->SetParent(window->body->get_transform(), false);
        vertical->set_childControlWidth(true); // Update width for all child objects
        vertical->set_childControlHeight(false); // Passively read hight from child object
        auto verticalFitter = vertical->get_gameObject()->AddComponent<ContentSizeFitter*>();
        verticalFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize); // Use the height from child objects

        // Declare common object names that may be reused
        HorizontalLayoutGroup* row;

        // Create each setting
        CreateFloatSetting(vertical->get_transform(), "Duration", "The length of time the Particle System is emitting particles. If the system is looping, this indicates the length of one cycle.", ParticleController::MainModule::get_duration(particleSystem), [particleSystem](float value){ParticleController::MainModule::set_duration(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Loop", "If true, the emission cycle will repeat after the duration.", ParticleController::MainModule::get_loop(particleSystem), [particleSystem](bool value){ParticleController::MainModule::set_loop(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Prewarm", "When played, a prewarmed system will be in a state as if it had emitted one loop cycle. Can only be used if the system is looping.", ParticleController::MainModule::get_prewarm(particleSystem), [particleSystem](bool value){ParticleController::MainModule::set_prewarm(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start delay", "Delay in seconds that this Particle System will wait before emitting particles. Cannot be used together with a prewarmed looping system.", ParticleController::MainModule::get_startDelay(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startDelay(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start lifetime", "Start lifetime in seconds, particle will die when its lifetime reaches 0.", ParticleController::MainModule::get_startLifetime(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startLifetime(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start speed", "The start speed of particles, applied in the starting direction.", ParticleController::MainModule::get_startSpeed(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startSpeed(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "3D Start size", "If enabled, you can control the size separately for each axis.", ParticleController::MainModule::get_startSize3D(particleSystem), [particleSystem](bool value){ParticleController::MainModule::set_startSize3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start size", "The start size of particles.", ParticleController::MainModule::get_startSize(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startSize(particleSystem, value);});
        row = BSML::Lite::CreateHorizontalLayoutGroup(vertical);
        CreateMinMaxCurveSetting(row->get_transform(), "X", "", ParticleController::MainModule::get_startSizeX(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startSizeX(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "Y", "", ParticleController::MainModule::get_startSizeY(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startSizeY(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "Z", "", ParticleController::MainModule::get_startSizeZ(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startSizeZ(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "3D Start rotation", "If enabled, you can control the rotation separately for each axis.", ParticleController::MainModule::get_startRotation3D(particleSystem), [particleSystem](bool value){ParticleController::MainModule::set_startRotation3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start rotation", "The start rotation of particles in degrees.", ParticleController::MainModule::get_startRotation(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startRotation(particleSystem, value);});
        row = BSML::Lite::CreateHorizontalLayoutGroup(vertical);
        CreateMinMaxCurveSetting(row->get_transform(), "X", "", ParticleController::MainModule::get_startRotationX(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startRotationX(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "Y", "", ParticleController::MainModule::get_startRotationY(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startRotationY(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "Z", "", ParticleController::MainModule::get_startRotationZ(particleSystem), [particleSystem](ParticleController::MinMaxCurve value){ParticleController::MainModule::set_startRotationZ(particleSystem, value);});
        CreateFloatSetting(vertical->get_transform(), "Flip rotation", "Cause some particles to spin in the opposite direction. (Set between 0 and 1, where a higher value causes more to flip)", ParticleController::MainModule::get_flipRotation(particleSystem), [particleSystem](float value){ParticleController::MainModule::set_flipRotation(particleSystem, value);});
        // TODO Crashes...?
        // CreateMinMaxGradientSetting(vertical->get_transform(), "Start color", "The start color of particles.", ParticleController::MainModule::get_startColor(particleSystem), [particleSystem](ParticleController::MinMaxGradient value){ParticleController::MainModule::set_startColor(particleSystem, value);});

        // Update size of the window
        window->SetDirty(true);

        return window;
    }

}