//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_RESOURCEMANAGER_H
#define HOMEWORK1_RESOURCEMANAGER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <glew.h>
#include <iostream>
#include <map>

enum ModelTypes {
    CUBE,
    SPHERE,
    BUNNY,
    MAID,
    CONE,
    CYLINDER,
    END_OF_TYPES_MARKER,
    TORUS,
    TEAPOT,
    PLANE,
    SKYBOX,
    QUAD,
};

// Define a structure to hold vertices and indices
struct ModelData {
    ModelTypes type;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colorVertices;
};

// Resource Manager class to manage loading and sharing of model data
class ResourceManager {
private:
    static inline ModelData* models[ModelTypes::END_OF_TYPES_MARKER] = {};



public:

    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    };

    static void generateBuiltinModels();

    static void addModel(const int modelIndex, ModelData* modelData) {


        models[modelIndex] = modelData;

        std::cout << "Model added: " << modelIndex << std::endl;
        std::cout << "Model added: " << models[modelIndex]->type << std::endl;
        std::cout << "Model added: " << models[modelIndex]->indices.size() << std::endl;

//        bufferModelData((ModelTypes) modelIndex, modelData);


    }


    static ModelData* getModel(const ModelTypes modelType) {
        return models[modelType];
    }

    static void generateExternalModels();

    void static bufferModelData(ModelTypes modelType, ModelData* modelData);

    static void loadModel(const std::string &filePath, ModelData* modelIndex);
};

void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint>& indices,
                    int subdivisions);

#endif //HOMEWORK1_RESOURCEMANAGER_H
