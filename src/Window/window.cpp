#include "Window/window.hpp"
#include "main.hpp"

#include "Window/createSettings.hpp"

#include "UI/utils.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "bsml/shared/Helpers/getters.hpp"

#include "VRUIControls/VRUIControls.hpp"

#include "HMUI/ImageView.hpp"
#include "HMUI/Screen.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/LayoutGroup.hpp"

DEFINE_TYPE(Flair::Window, Window);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

namespace Flair::Window {

    const float windowScale = 0.2f;
    const float windowTitleBarHeight = 6;
    const int windowPadding = 2;

    // DEBUG
    void Window::Update() {
        if(!tempHandle) return;

        get_transform()->set_position(tempHandle->handle->get_transform()->get_position());
        get_transform()->set_rotation(tempHandle->handle->get_transform()->get_rotation());
        get_transform()->Translate({0, -0.01f, 0}, get_transform());
    }

    void Window::Close() {
        if(tempHandle) GameObject::Destroy(tempHandle->get_gameObject());
        GameObject::Destroy(get_gameObject());
    }

    void Window::SetDirty() {
        // TODO Explore LayoutRebuilder https://github.com/Metalit/Qounters/blob/89c83517083aee1f33fb67813c5cc07266beec0c/src/utils.cpp#L599
        if(!_mainContainerFitter) _mainContainerFitter = mainContainer->GetComponent<ContentSizeFitter*>();
        _mainContainerFitter->SetDirty();

        if(!_canvasTransform) _canvasTransform = get_transform()->Find("Canvas")->GetComponent<RectTransform*>();
        if(!_mainContainerTransform) _mainContainerTransform = mainContainer->GetComponent<RectTransform*>();
        _canvasTransform->set_sizeDelta(_mainContainerTransform->get_rect().get_size());
    }
    void Window::SetDirty(bool nextFrame) {
        if(nextFrame) BSML::MainThreadScheduler::ScheduleNextFrame([this](){SetDirty();});
        else SetDirty();
    }
    void Window::SetDirty(float delaySeconds) {
        BSML::MainThreadScheduler::ScheduleAfterTime(delaySeconds, [this](){SetDirty();});
    }

    Window* CreateWindow(Window* parent, std::string title, Vector2 size) {
        GameObject* windowGO = GameObject::New_ctor("FlairWindow \"" + title + "\"");
        Transform* windowTransform = windowGO->get_transform();
        Window* window = windowGO->AddComponent<Window*>();
        window->parentWindow = parent;
        if(parent) parent->childWindows->Add(window);

        BSML::FloatingScreen* floatingScreen = BSML::FloatingScreen::CreateFloatingScreen({20, 20}, true, {0, 2, 2}, Quaternion::Euler({0, 0, 0}));
        floatingScreen->set_HandleSide(BSML::Side::Top);
        window->tempHandle = floatingScreen;

        GameObject* canvasGO = BSML::Lite::CreateCanvas();
        canvasGO->set_name("Canvas");
        RectTransform* canvasTransform = canvasGO->GetComponent<RectTransform*>();
        canvasTransform->SetParent(windowTransform, false);
        canvasTransform->set_anchorMin({0, 0});
        canvasTransform->set_anchorMax({0, 0});
        canvasTransform->set_pivot({0.5, 1});
        canvasTransform->set_anchoredPosition({0, 0});
        canvasTransform->set_sizeDelta({0, 0});
        float canvasScale = canvasTransform->get_localScale().x * windowScale;
        canvasTransform->set_localScale({canvasScale, canvasScale, canvasScale});
        canvasGO->AddComponent<Screen*>();
        canvasGO->GetComponent<VRUIControls::VRGraphicRaycaster*>()->_physicsRaycaster = BSML::Helpers::GetPhysicsRaycasterWithCache();
        canvasGO->AddComponent<CurvedCanvasSettings*>()->SetRadius(0);

        GameObject* mainContainerGO = GameObject::New_ctor("MainContainer");
        window->mainContainer = mainContainerGO;
        RectTransform* mainContainerTransform = mainContainerGO->AddComponent<RectTransform*>();
        mainContainerTransform->SetParent(canvasTransform, false);
        mainContainerTransform->set_anchorMin({0.5, 1});
        mainContainerTransform->set_anchorMax({0.5, 1});
        mainContainerTransform->set_pivot({0.5, 1});
        mainContainerTransform->set_anchoredPosition({0, 0});
        mainContainerTransform->set_sizeDelta(size);
        VerticalLayoutGroup* mainContainerLayoutG = mainContainerGO->AddComponent<VerticalLayoutGroup*>();
        mainContainerLayoutG->set_childControlWidth(true);
        mainContainerLayoutG->set_childControlHeight(false);
        mainContainerLayoutG->set_padding(RectOffset::New_ctor(windowPadding, windowPadding, windowPadding, windowPadding));
        mainContainerLayoutG->set_spacing(windowPadding);
        ContentSizeFitter* mainContainerFitter = mainContainerGO->AddComponent<ContentSizeFitter*>();
        mainContainerFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize);
        mainContainerGO->AddComponent<BSML::Backgroundable*>()->ApplyBackground("round-rect-panel");
        mainContainerGO->AddComponent<Touchable*>(); // TODO Disappears?

        GameObject* titleBarGO = GameObject::New_ctor("TitleBar");
        RectTransform* titleBarTransform = titleBarGO->AddComponent<RectTransform*>();
        titleBarTransform->SetParent(mainContainerTransform, false);
        titleBarTransform->set_sizeDelta({-1, windowTitleBarHeight});

        CurvedTextMeshPro* titleText = BSML::Lite::CreateText(titleBarTransform, title, {0, 0}, {0, 0});
        titleText->set_name("TitleText");
        titleText->set_horizontalAlignment(TMPro::HorizontalAlignmentOptions::Left);
        titleText->set_verticalAlignment(TMPro::VerticalAlignmentOptions::Middle);
        titleText->set_fontSize(4);
        titleText->set_overflowMode(TMPro::TextOverflowModes::Ellipsis);
        Object::DestroyImmediate(titleText->GetComponent<LayoutElement*>());
        RectTransform* titleTransform = titleText->GetComponent<RectTransform*>();
        titleTransform->set_anchorMin({0, 0});
        titleTransform->set_anchorMax({0.8, 1});
        titleTransform->set_pivot({0, 0.5});
        titleTransform->set_anchoredPosition({0, 0});
        titleTransform->set_sizeDelta({0, 0});

        Button* closeButton = Flair::UI::CreateFlatButton(titleBarTransform, "X", [window](){window->Close();});
        closeButton->set_name("CloseButton");
        RectTransform* closeButtonRect = closeButton->GetComponent<RectTransform*>();
        closeButtonRect->set_anchorMin({1, 1});
        closeButtonRect->set_anchorMax({1, 1});
        closeButtonRect->set_pivot({1, 1});
        closeButtonRect->set_anchoredPosition({0, 0});
        closeButtonRect->set_sizeDelta({windowTitleBarHeight, windowTitleBarHeight});

        GameObject* bodyGO = GameObject::New_ctor("Body");
        window->body = bodyGO;
        RectTransform* bodyTransform = bodyGO->AddComponent<RectTransform*>();
        bodyTransform->SetParent(mainContainerTransform, false);
        bodyTransform->set_sizeDelta(size);
        if(size.y == -1) {
            VerticalLayoutGroup* bodyLayoutG = bodyGO->AddComponent<VerticalLayoutGroup*>();
            bodyLayoutG->set_childControlWidth(true);
            bodyLayoutG->set_childControlHeight(false);
            ContentSizeFitter* bodyFitter = bodyGO->AddComponent<ContentSizeFitter*>();
            bodyFitter->set_verticalFit(__ContentSizeFitter__FitMode::PreferredSize);
        }

        return window;
    }

}