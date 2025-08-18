#include "logging.hpp"
#include "main.hpp"

#include "assimp/scene.h"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "System/Type.hpp"

using namespace UnityEngine;

void logHierarchy(const aiNode* node, const std::string& depthStr/* = ""*/) {
    aiVector3D scale;
    aiVector3D rotation;
    aiVector3D position;
    node->mTransformation.Decompose(scale, rotation, position);

    std::vector<int> meshIndices;
    for(int i = 0; i < node->mNumMeshes; i++) meshIndices.push_back(node->mMeshes[i]);

    PaperLogger.info("{}", depthStr);
    PaperLogger.info("{}Node: '{}'", depthStr, node->mName.C_Str());
    PaperLogger.info("{}- Position: ({:.1f}, {:.1f}, {:.1f})", depthStr, position.x, position.y, position.z);
    PaperLogger.info("{}- Rotation: ({:.1f}, {:.1f}, {:.1f})", depthStr, rotation.x, rotation.y, rotation.z);
    PaperLogger.info("{}- Scale: ({:.1f}, {:.1f}, {:.1f})", depthStr, scale.x, scale.y, scale.z);
    PaperLogger.info("{}- Meshes: {}", depthStr, meshIndices);

    if(node->mMetaData != nullptr) {
        for(int i = 0; i < node->mMetaData->mNumProperties; i++) {
            std::string_view key = node->mMetaData->mKeys[i].C_Str();
            aiMetadataType type = node->mMetaData->mValues[i].mType;
            // TODO: Convert void* to type
            PaperLogger.info("{}- MD {}", depthStr, i);
        }
    }

    for(int i = 0; i < node->mNumChildren; i++) {
        aiNode* child = node->mChildren[i];
        logHierarchy(child, depthStr + DEPTH_STR);
    }
}

void logHierarchy(Transform* transform, const int depthUp/* = 0*/, const int depthDown/* = 1000*/, const bool listComponents/* = false*/, std::string depthStr/* = ""*/) {
    DEFINE_HIERARCHY_FUNC_NAME_BUFFER("logHierarchy");
    if(depthUp > 0) {
        std::vector<Transform*> familyTree;
        Transform* currentTransform = transform;
        while(currentTransform->get_parent() && familyTree.size() < depthUp) {
            familyTree.push_back(currentTransform->get_parent());
            currentTransform = currentTransform->get_parent();
        }

        for(int i = 0; i < familyTree.size(); i++) {
            if(listComponents) PaperLogger.info("{}{}", HFNB, depthStr);
            PaperLogger.info("{}{}GameObject: '{}'", HFNB, depthStr, familyTree[i]->get_name());
            if(listComponents) logComponents(familyTree[i], depthStr);
            depthStr += DEPTH_STR;
        }

        depthStr += "   ";
    }

    if(listComponents) PaperLogger.info("{}{}", HFNB, depthStr);
    PaperLogger.info("{}{}GameObject: '{}'", HFNB, depthStr, transform->get_name());

    if(listComponents) logComponents(transform, depthStr);

    for(int i = 0; i < transform->get_childCount(); i++) {
        Transform* childTransform = transform->GetChild(i);
        logHierarchy(childTransform, 0, depthDown - 1, listComponents, depthStr + DEPTH_STR);
    }
}

void logComponents(UnityEngine::Transform* transform, const std::string& depthStr/* = ""*/) {
    DEFINE_HIERARCHY_FUNC_NAME_BUFFER("logComponents");
    ArrayW<Component*> components = transform->GetComponents<Component*>();
    for(int i = 0; i < components.size(); i++) {
        PaperLogger.info("{}{}- {}", HFNB, depthStr, components[i]->GetType()->get_NameOrDefault());
    }
}