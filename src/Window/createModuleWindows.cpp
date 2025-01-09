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
        Window* window = CreateWindow(parent, "Main Module", {50, -1});

        // Create a VerticalLayoutGroup that will automatically space the settings
        auto vertical = GameObject::New_ctor("Vertical")->AddComponent<UI::VerticalLayoutGroup*>();
        vertical->get_transform()->SetParent(window->body->get_transform(), false);
        vertical->set_childControlWidth(true); // Update width for all child objects
        vertical->set_childControlHeight(false); // Passively read hight from child object
        auto verticalFitter = vertical->get_gameObject()->AddComponent<ContentSizeFitter*>();
        verticalFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize); // Use the height from child objects

        // Declare common object names that may be reused
        GameObject* row;
        RectTransform* rowTransform;
        HorizontalLayoutGroup* rowLayout;

        // Create each setting
        CreateFloatSetting(vertical->get_transform(), "Duration", "The length of time the Particle System is emitting particles. If the system is looping, this indicates the length of one cycle.", ParticleInterface::MainModule::get_duration(particleSystem), [particleSystem](float value){ParticleInterface::MainModule::set_duration(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Loop", "If true, the emission cycle will repeat after the duration.", ParticleInterface::MainModule::get_loop(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_loop(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "Prewarm", "When played, a prewarmed system will be in a state as if it had emitted one loop cycle. Can only be used if the system is looping.", ParticleInterface::MainModule::get_prewarm(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_prewarm(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start delay", "Delay in seconds that this Particle System will wait before emitting particles. Cannot be used together with a prewarmed looping system.", ParticleInterface::MainModule::get_startDelay(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startDelay(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start lifetime", "Start lifetime in seconds, particle will die when its lifetime reaches 0.", ParticleInterface::MainModule::get_startLifetime(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startLifetime(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start speed", "The start speed of particles, applied in the starting direction.", ParticleInterface::MainModule::get_startSpeed(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSpeed(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "3D Start size", "If enabled, you can control the size separately for each axis.", ParticleInterface::MainModule::get_startSize3D(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_startSize3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start size", "The start size of particles.", ParticleInterface::MainModule::get_startSize(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSize(particleSystem, value);});
        row = GameObject::New_ctor("3MinMaxCurveSetting");
        row->get_transform()->SetParent(vertical->get_transform(), false);
        rowTransform = row->AddComponent<RectTransform*>();
        rowTransform->set_sizeDelta({0, 8});
        rowLayout = row->AddComponent<HorizontalLayoutGroup*>();
        rowLayout->set_childControlWidth(true);
        rowLayout->set_childControlHeight(false);
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startSizeX(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeX(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startSizeY(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeY(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startSizeZ(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startSizeZ(particleSystem, value);});
        CreateBoolSetting(vertical->get_transform(), "3D Start rotation", "If enabled, you can control the rotation separately for each axis.", ParticleInterface::MainModule::get_startRotation3D(particleSystem), [particleSystem](bool value){ParticleInterface::MainModule::set_startRotation3D(particleSystem, value);});
        CreateMinMaxCurveSetting(vertical->get_transform(), "Start rotation", "The start rotation of particles in degrees.", ParticleInterface::MainModule::get_startRotation(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotation(particleSystem, value);});
        row = GameObject::New_ctor("3MinMaxCurveSetting");
        row->get_transform()->SetParent(vertical->get_transform(), false);
        rowTransform = row->AddComponent<RectTransform*>();
        rowTransform->set_sizeDelta({0, 8});
        rowLayout = row->AddComponent<HorizontalLayoutGroup*>();
        rowLayout->set_childControlWidth(true);
        rowLayout->set_childControlHeight(false);
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startRotationX(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationX(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startRotationY(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationY(particleSystem, value);});
        CreateMinMaxCurveSetting(row->get_transform(), "", "", ParticleInterface::MainModule::get_startRotationZ(particleSystem), [particleSystem](ParticleInterface::MinMaxCurve value){ParticleInterface::MainModule::set_startRotationZ(particleSystem, value);});
        CreateFloatSetting(vertical->get_transform(), "Flip rotation", "Cause some particles to spin in the opposite direction. (Set between 0 and 1, where a higher value causes more to flip)", ParticleInterface::MainModule::get_flipRotation(particleSystem), [particleSystem](float value){ParticleInterface::MainModule::set_flipRotation(particleSystem, value);});
        // TODO Crashes...?
        // CreateMinMaxGradientSetting(vertical->get_transform(), "Start color", "The start color of particles.", ParticleInterface::MainModule::get_startColor(particleSystem), [particleSystem](ParticleInterface::MinMaxGradient value){ParticleInterface::MainModule::set_startColor(particleSystem, value);});

        // Update size of the window
        window->SetDirty(true);

        return window;
    }

}