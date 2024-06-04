#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Primitives.h"
#include "renderer/Shader.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture roughness;
    Texture ambientOcclusion;
    void draw(Shader& shader) const;

    GLuint VAO, VBO, EBO;

    void setupMesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);


};

struct Model {
    std::vector<Mesh> meshes;
    void draw(Shader& shader);
};

Mesh processMesh(aiMesh* mesh, const aiScene* scene);

void processNode(aiNode* node, const aiScene* scene, Model& model);

Model loadModel(const std::string& path);

std::vector<Texture> loadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, TextureType typeName);
unsigned int TextureFromFile(const char *path);
unsigned int TextureFromEmbeddedTexture(const aiScene *scene, const char *path);


#endif MODELLOADER_H