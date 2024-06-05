#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <glew.h>
#include "modelLoader.h"
#include "stb_image.h"
#include "renderer/Renderer.h"
#include "Model.h"
#include "Mesh.h"


Model loadModel(const std::string &objPath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        throw std::runtime_error("Failed to load model");
    }

    Model model;
    processNode(scene->mRootNode, scene, model, objPath);

    return model;
}

void processNode(aiNode *node, const aiScene *scene, Model &model, const std::string &objPath) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(processMesh(mesh, scene, objPath));
        std::cout << "Mesh processed" << std::endl;
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, model, objPath);
    }
}

Mesh processMesh(aiMesh *mesh, const aiScene *scene, const std::string &objPath) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
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

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> albedoMaps = loadMaterialTextures(objPath, scene, material, aiTextureType_DIFFUSE, ALBEDO);
        textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());

        std::vector<Texture> normalMaps = loadMaterialTextures(objPath, scene, material, aiTextureType_NORMALS, NORMAL);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> metallicMaps = loadMaterialTextures(objPath, scene, material, aiTextureType_METALNESS, METALLIC);
        textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());

        std::vector<Texture> roughnessMaps = loadMaterialTextures(objPath, scene, material, aiTextureType_DIFFUSE_ROUGHNESS, ROUGHNESS);
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

        std::vector<Texture> aoMaps = loadMaterialTextures(objPath, scene, material, aiTextureType_AMBIENT_OCCLUSION, AMBIENT_OCCLUSION);
        textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
    }

    std::cout << "Vertices: " << vertices.size() << std::endl;

    for (unsigned int i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        glm::vec2 deltaUV1 = v1.textureCoordinates - v0.textureCoordinates;
        glm::vec2 deltaUV2 = v2.textureCoordinates - v0.textureCoordinates;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);

        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;

        v0.bitangent = bitangent;
        v1.bitangent = bitangent;
        v2.bitangent = bitangent;
    }

    std::cout << "Textures: " << textures.size() << std::endl;

    return {vertices, indices, textures};
}

std::vector<Texture> loadMaterialTextures(const std::string &objPath, const aiScene* scene, aiMaterial *mat, aiTextureType type, TextureType typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        if (str.C_Str()[0] == '*') {
            // This is an embedded texture
            texture.id = TextureFromEmbeddedTexture(scene, str.C_Str());
            texture.path = str.C_Str();
        } else {
            // This is a file path
            texture.id = TextureFromFile(str.C_Str(), objPath);
            std::string filename = std::string(str.C_Str());
            // Get the directory of the obj file
            size_t lastSlash = objPath.find_last_of('/');
            std::string directory = objPath.substr(0, lastSlash + 1);
            texture.path = directory + filename;
        }
        texture.type = typeName;
        textures.push_back(texture);
        std::cout << "Texture loaded: " << texture.path << std::endl;
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const std::string &objPath) {
    std::string filename = std::string(path);
    // Get the directory of the obj file
    size_t lastSlash = objPath.find_last_of('/');
    std::string directory = objPath.substr(0, lastSlash + 1);
    filename = directory + filename;
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

        std::cout << "Texture loaded: " << filename << std::endl;

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

    std::cout << "Texture ID: " << textureID << std::endl;

    return textureID;
}

unsigned int TextureFromEmbeddedTexture(const aiScene *scene, const char *path) {
    // Extract the texture index from the path (e.g., "*0" -> 0)
    int textureIndex = std::stoi(std::string(path).substr(1));

    // Get the embedded texture from the scene
    aiTexture *texture = scene->mTextures[textureIndex];
    if (texture) {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        if (texture->mHeight == 0) {
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
                } else if (nrComponents == 3) {
                    format = GL_RGB;
                } else if (nrComponents == 4) {
                    format = GL_RGBA;
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
            // Uncompressed texture (e.g., BMP)
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->mWidth, texture->mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture->pcData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }


        return textureID;
    }

    return 0;
}

void Model::draw(Shader& shader, glm::mat4 transform) {

    for (auto &mesh : meshes) {
        mesh.draw(shader, transform);
    }

}

void Model::drawInstanced(Shader &shader, const std::vector<glm::mat4> &transforms) {

    for (auto &mesh : meshes) {
        mesh.drawInstanced(shader, const_cast<std::vector<glm::mat4> &>(transforms));
    }

}


void Mesh::drawInstanced(Shader &shader, std::vector<glm::mat4> &transforms) {
    setupInstancedBuffer(transforms);

    shader.setBool("isInstanced", true);

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

    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, transforms.size());
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupInstancedBuffer(std::vector<glm::mat4> &transforms) {
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), &transforms[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::draw(Shader &shader, glm::mat4 transform) const {

    shader.setMat4("model", transform);
    shader.setBool("isInstanced", false);

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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::vector<Texture>& textures)
        : vertices(std::move(vertices)), indices(std::move(indices)) {
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

