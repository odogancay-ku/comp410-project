#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

void processNode(aiNode* node, const aiScene* scene, Model& model);

Model loadModel(const std::string& path);

#endif