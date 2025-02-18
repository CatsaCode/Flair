#pragma once

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"

namespace Flair::UI {

    UnityEngine::UI::Button* CreateFlatButton(UnityEngine::Transform* parent, std::string_view text, std::function<void()> onClick);

}