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
    if(depthUp > 0) {
        std::vector<Transform*> familyTree;
        Transform* currentTransform = transform;
        while(currentTransform->get_parent() && familyTree.size() < depthUp) {
            familyTree.push_back(currentTransform->get_parent());
            currentTransform = currentTransform->get_parent();
        }

        for(int i = 0; i < familyTree.size(); i++) {
            PaperLogger.info("{}GameObject: '{}'", depthStr, familyTree[i]->get_name());
            depthStr += DEPTH_STR;
        }
    }

    if(listComponents) PaperLogger.info("{}", depthStr);
    PaperLogger.info("{}GameObject: '{}'", depthStr, transform->get_name());

    if(listComponents) {
        ArrayW<Component*> components = transform->GetComponents<Component*>();
        for(int i = 0; i < components.size(); i++) {
            PaperLogger.info("{}- {}", depthStr, components[i]->GetType()->get_NameOrDefault());
        }
    }

    for(int i = 0; i < transform->get_childCount(); i++) {
        Transform* childTransform = transform->GetChild(i);
        logHierarchy(childTransform, 0, depthDown - 1, listComponents, depthStr + DEPTH_STR);
    }
}