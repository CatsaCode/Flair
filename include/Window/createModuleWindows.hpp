#pragma once

#include "Window/window.hpp"

#include "UnityEngine/ParticleSystem.hpp"

namespace Flair::Window {

    Window* CreateMainModuleWindow(Window* parent, UnityEngine::ParticleSystem* particleSystem);

}