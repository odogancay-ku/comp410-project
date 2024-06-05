#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Primitives.h"
#include "renderer/Shader.h"
#include "Model.h"
#include "Mesh.h"


Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& objPath);

void processNode(aiNode* node, const aiScene* scene, Model& model, const std::string& objPath);

Model loadModel(const std::string& path);

std::vector<Texture> loadMaterialTextures(const std::string &objPath,const aiScene *scene, aiMaterial *mat, aiTextureType type, TextureType typeName);
unsigned int TextureFromFile(const char *path, const std::string & objPath);
unsigned int TextureFromEmbeddedTexture(const aiScene *scene, const char *path);


#endif MODELLOADER_H