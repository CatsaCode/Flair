#pragma once

#include "assimp/scene.h"

#define LOG_GARBAGE_COLLECTION_ISSUES   true

#define LOG_SCENE_HIERARCHY             true
#define LOG_MESH_DATA                   true
#define LOG_TEXTURE_DATA                true
#define LOG_MATERIAL_DATA               true
#define LOG_PREFAB_DATA                 true
#define LOG_NODE_TO_GAMEOBJECT          true

#define LOG_EXPORT                      true

#define DEPTH_STR "|  "

void logHierarchy(const aiNode* node, const std::string& depthStr = "");