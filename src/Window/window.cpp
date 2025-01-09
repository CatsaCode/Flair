#include "Window/window.hpp"
#include "main.hpp"

#include "Window/createSettings.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"

#include "HMUI/ImageView.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/LayoutGroup.hpp"

const float windowScale = 0.2f;

DEFINE_TYPE(Flair::Window, Window);

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace Flair::Window {

    // DEBUG
    void Window::Update() {
        if(!tempHandle) return;

        if(!_rectTransform) _rectTransform = GetComponent<RectTransform*>();
        if(!_rectTransform) {PaperLogger.error("Window::Update() - Could not get RectTransform"); return;};

        get_transform()->set_position(tempHandle->handle->get_transform()->get_position());
        get_transform()->set_rotation(tempHandle->handle->get_transform()->get_rotation());
        get_transform()->Translate({0, -_rectTransform->get_sizeDelta().y * 0.0031f, 0}, get_transform());
    }

    // Destroy the window object
    void Window::Close() {
        if(tempHandle) GameObject::Destroy(tempHandle->get_gameObject());
        GameObject::Destroy(get_gameObject());
    }

    // Call ContentSizeFitter::SetDirty() so that the window's size is recalculated
    void Window::SetDirty() {
        // TODO Explore LayoutRebuilder https://github.com/Metalit/Qounters/blob/89c83517083aee1f33fb67813c5cc07266beec0c/src/utils.cpp#L599
        if(!_fitter) _fitter = GetComponent<ContentSizeFitter*>();
        if(!_fitter) {PaperLogger.error("Window::SetDirty() - Could not get ContentSizeFitter"); return;};

        _fitter->SetDirty();
    }
    void Window::SetDirty(bool nextFrame) {
        if(nextFrame) BSML::MainThreadScheduler::ScheduleNextFrame([this](){SetDirty();});
        else SetDirty();
    }
    void Window::SetDirty(float delaySeconds) {
        BSML::MainThreadScheduler::ScheduleAfterTime(delaySeconds, [this](){SetDirty();});
    }

    Window* CreateWindow(Window* parent, std::string title, Vector2 size) {
        // Create canvas
        GameObject* windowCanvas = BSML::Lite::CreateCanvas();
        windowCanvas->set_name("FlairWindow \"" + title + "\"");
        
        // Set scale of the pixels
        float canvasScale = windowCanvas->get_transform()->get_localScale().x * windowScale;
        windowCanvas->get_transform()->set_localScale({canvasScale, canvasScale, canvasScale});

        // Set up Window component
        Window* window = windowCanvas->AddComponent<Window*>();

        // Create child-parent link
        window->parentWindow = parent;
        if(parent) parent->childWindows->Add(window);

        // DEBUG Create temporary handle
        BSML::FloatingScreen* floatingScreen = BSML::FloatingScreen::CreateFloatingScreen({20, 20}, true, {0, 2, 2}, Quaternion::Euler({0, 0, 0}));
        floatingScreen->set_HandleSide(BSML::Side::Top);
        window->tempHandle = floatingScreen;

        // Set size of the window
        const float titleBarHeight = 8;
        const float titleBarPadding = 1;
        windowCanvas->GetComponent<RectTransform*>()->set_sizeDelta({size.x, size.y + titleBarHeight});

        auto windowCanvasVertical = windowCanvas->AddComponent<VerticalLayoutGroup*>();
        windowCanvasVertical->set_childControlWidth(true); // Scale titleBar and body to the same width
        windowCanvasVertical->set_childControlHeight(false); // Don't control children, just passively get their height information
        // Update windowCanvas's RectTransform to fit both the titleBar and the body
        auto windowCanvasFitter = windowCanvas->AddComponent<ContentSizeFitter*>();
        windowCanvasFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize);

        // Give the window a background
        windowCanvas->AddComponent<BSML::Backgroundable*>()->ApplyBackground("round-rect-panel");

        // Add a GameObject that will house the objects in the titleBar
        auto titleBar = GameObject::New_ctor("TitleBar");
        titleBar->get_transform()->SetParent(windowCanvas->get_transform(), false);
        auto titleBarTransform = titleBar->AddComponent<RectTransform*>();
        titleBarTransform->set_sizeDelta({-1, titleBarHeight}); // Width is automatic from windowCanvasVertical

        // Create close button
        // Start with BSML template
        auto closeButton = BSML::Lite::CreateUIButton(titleBar->get_transform(), "X", [window](){window->Close();});
        // Remove padding so the button can be completely square
        auto closeButtonLayoutG = closeButton->GetComponent<LayoutGroup*>();
        closeButtonLayoutG->set_padding(RectOffset::New_ctor(0, 0, 0, 0));
        // Set the size of the button
        auto closeButtonLayoutE = closeButton->GetComponent<LayoutElement*>();
        closeButtonLayoutE->set_preferredWidth(titleBarHeight - titleBarPadding * 2);
        closeButtonLayoutE->set_preferredHeight(titleBarHeight - titleBarPadding * 2);
        // Position the button in the top right corner
        auto closeButtonRect = closeButton->GetComponent<RectTransform*>();
        closeButtonRect->set_anchorMin({1, 1});
        closeButtonRect->set_anchorMax({1, 1});
        closeButtonRect->set_pivot({1, 1});
        closeButtonRect->set_anchoredPosition({-titleBarPadding, -titleBarPadding});
        for(auto imageView : closeButton->GetComponentsInChildren<HMUI::ImageView*>()) {
            // Turn the parallelogram into a basic square
            imageView->_skew = 0;
            // Remove the underline
            if(imageView->get_name() == "Underline") GameObject::Destroy(imageView->get_gameObject());
        }

        // Add a GameObject that will house the objects in the titleBar
        window->body = GameObject::New_ctor("Body");
        window->body->get_transform()->SetParent(windowCanvas->get_transform(), false);
        auto bodyTransform = window->body->AddComponent<RectTransform*>();
        bodyTransform->set_sizeDelta(size);

        // If height was set as -1, interpret that as automatically change height to fit content
        if(size.y == -1) {
            auto bodyVertical = window->body->AddComponent<VerticalLayoutGroup*>();
            bodyVertical->set_childControlWidth(true); // A LayoutGroup disables anchors in child objects. Make sure the width can still carry through
            bodyVertical->set_childControlHeight(false); // Don't control children, just passively get their height
            // Update the body's RectTransform to fit its children vertically
            auto bodyFitter = window->body->AddComponent<ContentSizeFitter*>();
            bodyFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize);
        }

        return window;
    }

}