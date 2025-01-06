#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/GameObject.hpp"

DECLARE_CLASS_CODEGEN(Flair::Window, Window, UnityEngine::MonoBehaviour,
    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, Update); // DEBUG

    public:
        Window* parentWindow;
        std::vector<Window*> childWindows;
        BSML::FloatingScreen* tempHandle; // DEBUG
        UnityEngine::GameObject* body;

        void Close();

        void SetDirty();
        void SetDirty(bool nextFrame);
        void SetDirty(float delaySeconds);
);

namespace Flair::Window {
    Window* CreateWindow(Flair::Window::Window* window, std::string name, UnityEngine::Vector2 size);
}