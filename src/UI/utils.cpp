#include "UI/utils.hpp"
#include "main.hpp"

#include "bsml/shared/BSML.hpp"

#include "HMUI/ImageView.hpp"
#include "HMUI/StackLayoutGroup.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

namespace Flair::UI {

    void LogHierarchy(Transform* root) {
        for(int i = 0; i < root->GetChildCount(); i++) PaperLogger.info("Name: {}", root->GetChild(i)->get_name());
    }

    Button* CreateFlatButton(UnityEngine::Transform* parent, std::string_view text, std::function<void()> onClick) {
        Button* button = BSML::Lite::CreateUIButton(parent, text, onClick);
        button->set_name("FlatButton");
        Object::DestroyImmediate(button->GetComponent<LayoutElement*>());
        Object::DestroyImmediate(button->GetComponent<ContentSizeFitter*>());
        Object::DestroyImmediate(button->GetComponent<StackLayoutGroup*>());
        RectTransform* buttonTransform = button->GetComponent<RectTransform*>();

        RectTransform* textTransform = buttonTransform->Find("Content/Text")->GetComponent<RectTransform*>();
        textTransform->SetParent(buttonTransform, false);
        textTransform->set_anchorMin({0, 0});
        textTransform->set_anchorMax({1, 1});
        textTransform->set_pivot({0, 0});
        textTransform->set_anchoredPosition({0, 0});
        textTransform->set_sizeDelta({0, 0});

        GameObject::Destroy(buttonTransform->Find("Content")->get_gameObject());

        ImageView* backgroundImage = buttonTransform->Find("BG")->GetComponent<ImageView*>();
        backgroundImage->_skew = 0;
        RectTransform* backgroundTransform = backgroundImage->GetComponent<RectTransform*>();
        backgroundTransform->set_anchorMin({0, 0});
        backgroundTransform->set_anchorMax({1, 1});
        backgroundTransform->set_pivot({0, 0});
        backgroundTransform->set_anchoredPosition({0, 0});
        backgroundTransform->set_sizeDelta({0, 0});

        GameObject::Destroy(buttonTransform->Find("Underline")->get_gameObject());

        return button;
    }

}