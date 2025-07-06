#include "project.hpp"
#include "main.hpp"
#include "logging.hpp"

#include "assimp/Exporter.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "UnityEngine/Mesh.hpp"

#include <cstring>

using namespace Flair;
using namespace UnityEngine;

void Project::exportToFile(std::string_view filePath) {
    if(LOG_EXPORT) PaperLogger.info("Exporting to {}", filePath);

    if(LOG_EXPORT) PaperLogger.info("Setting up basic scene...");
    aiScene scene;
    scene.mRootNode = new aiNode("ROOT");

    // exportMeshes(scene);
    // scene.mRootNode->mMeshes = new unsigned int[1]{0};
    // scene.mRootNode->mNumMeshes = 1;
    exportMaterials(scene);

    if(LOG_EXPORT) PaperLogger.info("Exporting scene...");
    Assimp::Exporter exporter;
    exporter.Export(&scene, "gltf", std::string(filePath), aiProcess_ValidateDataStructure);

    if(strlen(exporter.GetErrorString()) > 0) {
        PaperLogger.error("{}", exporter.GetErrorString());
        return;
    }

    PaperLogger.info("Export successful");
}

void Project::exportMeshes(aiScene& scene) {
    if(LOG_EXPORT) PaperLogger.info("# Meshes: {}", meshes.size());
    scene.mMeshes = new aiMesh*[meshes.size()];
    scene.mNumMeshes = meshes.size();

    for(int i = 0; i < meshes.size(); i++) {
        if(LOG_EXPORT) PaperLogger.info("Setting up mesh {}...", i);
        Mesh* unityMesh = meshes[i].ptr();

        aiMesh* mesh = new aiMesh();
        scene.mMeshes[i] = mesh;

        ArrayW<Vector3> vertices = unityMesh->get_vertices();
        mesh->mVertices = new aiVector3D[vertices.size()];
        mesh->mNumVertices = vertices.size();
        for(int j = 0; j < vertices.size(); j++) {
            Vector3 vertex = vertices[j];
            mesh->mVertices[j] = aiVector3D(vertex.x, vertex.y, vertex.z);
        }

        ArrayW<int> triangles = unityMesh->get_triangles();
        mesh->mFaces = new aiFace[triangles.size() / 3];
        mesh->mNumFaces = triangles.size() / 3;
        for(int j = 0; j < triangles.size(); j += 3) {
            aiFace& face = mesh->mFaces[j / 3] = aiFace();
            face.mIndices = new unsigned int[3];
            face.mNumIndices = 3;
            face.mIndices[0] = triangles[j + 2];
            face.mIndices[1] = triangles[j + 1];
            face.mIndices[2] = triangles[j + 0];
        }

        // if(mesh->HasNormals()) {
        //     ArrayW<Vector3> normals (mesh->mNumVertices);
        //     for(int j = 0; j < mesh->mNumVertices; j++) {
        //         aiVector3D& normal = mesh->mNormals[j];
        //         if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Normal #: {}, Normal: ({:.2f}, {:.2f}, {:.2f})", j, normal.x, normal.y, normal.z);
        //         normals[j] = Vector3(normal.x, normal.y, normal.z);
        //     }
        //     unityMesh->set_normals(normals);
        // } else {
        //     if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Recalculating normals...");
        //     unityMesh->RecalculateNormals();
        // }

        // // TODO Import tangent data from assimp
        // if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Recalculating tangents...");
        // unityMesh->RecalculateTangents();

        // if(mesh->HasVertexColors(0)) {
        //     ArrayW<Color> colors (mesh->mNumVertices);
        //     for(int j = 0; j < mesh->mNumVertices; j++) {
        //         aiColor4D& color = mesh->mColors[i][j];
        //         if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Color #: {}, Color: ({:.2f}, {:.2f}, {:.2f}, {:.2f})", j, color.r, color.g, color.b, color.a);
        //         colors[j] = Color(color.r, color.g, color.b, color.a);
        //     }
        //     unityMesh->set_colors(colors);
        // }

        // for(int j = 0; j < 8; j++) {
        //     if(!mesh->HasTextureCoords(j)) continue;
        //     ArrayW<Vector2> uvs (mesh->mNumVertices);
        //     for(int k = 0; k < mesh->mNumVertices; k++) {
        //         aiVector3D& uv = mesh->mTextureCoords[j][k];
        //         if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "UV #: {}, Vertex #: {}, UV: ({:.2f}, {:.2f}, {:.2f})", j, k, uv.x, uv.y, uv.z);
        //         uvs[k] = Vector2(uv.x, uv.y);
        //     }
        //     unityMesh->SetUVs(j, uvs);
        // }

        // if(LOG_MESH_DATA) PaperLogger.info(DEPTH_STR "Material #: {}", mesh->mMaterialIndex);

        // meshes.push_back(unityMesh);
    }
}

void Project::exportMaterials(aiScene& scene) {
    if(LOG_EXPORT) PaperLogger.info("# Materials: {}", materials.size());
     
    scene.mMaterials = new aiMaterial*[1];
    scene.mNumMaterials = 1;
    scene.mMaterials[0] = new aiMaterial();
}