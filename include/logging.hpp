#pragma once

#include "assimp/scene.h"

#define LOG_GARBAGE_COLLECTION_ISSUES   true

#define LOG_LOAD_MODEL                  true     // Report each step while loading a new 3D model
#define LOG_LOAD_MODEL_SCENE            true     // Hierarchy log the root node when loading a new 3D model
#define LOG_A2U_MESH_DATA               false    // During assimp to Unity conversion, log mesh vertices, normals, UVs, etc.
#define LOG_A2U_TEXTURE_INFO            true     // During assimp to Unity conversion, log texture name, file type, resolution, etc.
#define LOG_A2U_MATERIAL_INFO           true
#define LOG_A2U_MATERIAL_DATA           false    
#define LOG_A2U_NODE_DATA               true     // During assimp to Unity conversion, log node transform and component information

#define LOG_MESH_DATA                   true
#define LOG_TEXTURE_DATA                true
#define LOG_MATERIAL_DATA               true
#define LOG_PREFAB_DATA                 true
#define LOG_NODE_TO_GAMEOBJECT          true

#define LOG_EXPORT                      true

#define DEPTH_STR "|  "

void logHierarchy(const aiNode* node, const std::string& depthStr = "");