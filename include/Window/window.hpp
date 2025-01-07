#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

DECLARE_CLASS_CODEGEN(Flair::Window, Window, UnityEngine::MonoBehaviour,
    // Frequently used components
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityW<UnityEngine::RectTransform>, _rectTransform);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityW<UnityEngine::UI::ContentSizeFitter>, _fitter);


    // Window relationships
    DECLARE_INSTANCE_FIELD(UnityW<Window>, parentWindow);
    DECLARE_INSTANCE_FIELD(ListW<UnityW<Window>>, childWindows);
    // DEBUG Parentless FloatingScreen whose handle is borrowed for the time being
    DECLARE_INSTANCE_FIELD(UnityW<BSML::FloatingScreen>, tempHandle);
    // UI objects
    DECLARE_INSTANCE_FIELD(UnityW<UnityEngine::GameObject>, body);


    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, Update); // DEBUG
    DECLARE_INSTANCE_METHOD(void, Close);


    public:
        // Force the Window's layout to rebuild
        void SetDirty();
        void SetDirty(bool nextFrame);
        void SetDirty(float delaySeconds);
);

namespace Flair::Window {
    Window* CreateWindow(Flair::Window::Window* window, std::string title, UnityEngine::Vector2 size);
}