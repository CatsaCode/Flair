#include "logging.hpp"
#include "main.hpp"

#include "assimp/scene.h"

void logHierarchy(const aiNode* node, const std::string& depthStr/* = ""*/) {
    aiVector3D scale;
    aiVector3D rotation;
    aiVector3D position;
    node->mTransformation.Decompose(scale, rotation, position);

    std::vector<int> meshIndices;
    for(int i = 0; i < node->mNumMeshes; i++) meshIndices.push_back(node->mMeshes[i]);

    PaperLogger.info("{}", depthStr);
    PaperLogger.info("{}Name: '{}'", depthStr, node->mName.C_Str());
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