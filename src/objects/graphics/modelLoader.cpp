#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <glew.h>
#include <gl.h>
#include "modelLoader.h"
#include "stb_image.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"

unsigned int TextureFromFile(const char *path);
unsigned int TextureFromEmbeddedTexture(const aiScene *scene, const char *path);

Model loadModel(const std::string &objPath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        throw std::runtime_error("Failed to load model");
    }

    Model model;
    processNode(scene->mRootNode, scene, model);

    return model;
}

void processNode(aiNode *node, const aiScene *scene, Model &model) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, model);
    }
}

Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoordinates = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::cout << "Mesh has " << mesh->mMaterialIndex+1 << " materials" << std::endl;

    if (mesh->mMaterialIndex >= 0) {
        std::cout << "Loading materials" << std::endl;
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(scene, material, aiTextureType_SPECULAR, SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> baseColorMaps = loadMaterialTextures(scene, material, aiTextureType_BASE_COLOR, ALBEDO);
        textures.insert(textures.end(), baseColorMaps.begin(), baseColorMaps.end());

        std::vector<Texture> roughnessMaps = loadMaterialTextures(scene, material, aiTextureType_DIFFUSE_ROUGHNESS, ROUGHNESS);
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

        std::vector<Texture> metalnessMaps = loadMaterialTextures(scene, material, aiTextureType_METALNESS, METALLIC);
        textures.insert(textures.end(), metalnessMaps.begin(), metalnessMaps.end());

        std::vector<Texture> emissiveMaps = loadMaterialTextures(scene, material, aiTextureType_EMISSIVE, EMISSIVE);
        textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(scene, material, aiTextureType_NORMALS, NORMAL);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps = loadMaterialTextures(scene, material, aiTextureType_HEIGHT, HEIGHT);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::vector<Texture> ambientOcclusionMaps = loadMaterialTextures(scene, material, aiTextureType_AMBIENT_OCCLUSION, AMBIENT_OCCLUSION);
        textures.insert(textures.end(), ambientOcclusionMaps.begin(), ambientOcclusionMaps.end());

        std::vector<Texture> reflectionMaps = loadMaterialTextures(scene, material, aiTextureType_REFLECTION, REFLECTION);
        textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());

        std::vector<Texture> lightMapMaps = loadMaterialTextures(scene, material, aiTextureType_LIGHTMAP, LIGHT_MAP);
        textures.insert(textures.end(), lightMapMaps.begin(), lightMapMaps.end());

        std::vector<Texture> opacityMaps = loadMaterialTextures(scene, material, aiTextureType_OPACITY, OPACITY);
        textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());

        std::vector<Texture> unknownMaps = loadMaterialTextures(scene, material, aiTextureType_UNKNOWN, UNKNOWN);
        textures.insert(textures.end(), unknownMaps.begin(), unknownMaps.end());

        // Load other texture types as needed
    }

    Mesh newMesh(vertices, indices, textures);
    return newMesh;
}

std::vector<Texture> loadMaterialTextures(const aiScene* scene, aiMaterial *mat, aiTextureType type, TextureType typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        std::cout << "Loading texture " << i << std::endl;
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << str.C_Str() << std::endl;
        Texture texture;
        if (str.C_Str()[0] == '*') {
            // This is an embedded texture
            texture.id = TextureFromEmbeddedTexture(scene, str.C_Str());
        } else {
            // This is a file path
            texture.id = TextureFromFile(str.C_Str());
        }
        texture.path = str.C_Str();
        texture.type = typeName;
        textures.push_back(texture);
    }
    return textures;
}

unsigned int TextureFromFile(const char *path) {
    std::string filename = std::string(path);
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int TextureFromEmbeddedTexture(const aiScene *scene, const char *path) {
    // Extract the texture index from the path (e.g., "*0" -> 0)
    int textureIndex = std::stoi(std::string(path).substr(1));
    std::cout << "Texture index: " << textureIndex << std::endl;

    // Get the embedded texture from the scene
    aiTexture *texture = scene->mTextures[textureIndex];
    if (texture) {
        std::cout << "Texture width: " << texture->mWidth << std::endl;
        std::cout << "Texture height: " << texture->mHeight << std::endl;
        unsigned int textureID;
        glGenTextures(1, &textureID);

        if (texture->mHeight == 0) {
            std::cout << "Embedded texture is compressed" << std::endl;
            // Compressed texture (e.g., PNG, JPEG)
            int width, height, nrComponents;
            unsigned char *data = stbi_load_from_memory(
                    reinterpret_cast<unsigned char*>(texture->pcData),
                    texture->mWidth,
                    &width, &height, &nrComponents, 0);
            if (data) {
                GLenum format;
                if (nrComponents == 1) {
                    format = GL_RED;
                    std::cout << "Format: GL_RED" << std::endl;
                } else if (nrComponents == 3) {
                    format = GL_RGB;
                    std::cout << "Format: GL_RGB" << std::endl;
                } else if (nrComponents == 4) {
                    format = GL_RGBA;
                    std::cout << "Format: GL_RGBA" << std::endl;
                }


                glBindTexture(GL_TEXTURE_2D, textureID);
                checkOpenGLError("TextureFromEmbeddedTexture");
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            } else {
                std::cerr << "Embedded texture failed to load" << std::endl;
                stbi_image_free(data);
            }
        } else {
            std::cout << "Embedded texture is uncompressed" << std::endl;
            // Uncompressed texture (e.g., BMP)
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->mWidth, texture->mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture->pcData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        std::cout << "Embedded texture loaded" << std::endl;

        return textureID;
    }

    return 0;
}

void Model::draw(Shader& shader) {

    for (auto &mesh : meshes) {
        mesh.draw(shader);
    }

}

void Mesh::draw(Shader &shader) const {
    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id);
    shader.setInt("material.albedo", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal.id);
    shader.setInt("material.normal", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic.id);
    shader.setInt("material.metallic", 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness.id);
    shader.setInt("material.roughness", 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ambientOcclusion.id);
    shader.setInt("material.ao", 4);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : vertices(vertices), indices(indices) {
    for (auto &texture : textures) {
        switch (texture.type) {
            case ALBEDO:
                albedo = texture;
                break;
            case NORMAL:
                normal = texture;
                break;
            case METALLIC:
                metallic = texture;
                break;
            case ROUGHNESS:
                roughness = texture;
                break;
            case AMBIENT_OCCLUSION:
                ambientOcclusion = texture;
                break;
            default:
                break;
        }
    }
    setupMesh();
}
