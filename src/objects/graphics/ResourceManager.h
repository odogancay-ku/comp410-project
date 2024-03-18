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
    END_OF_TYPES_MARKER,
    CONE,
    CYLINDER,
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

    std::vector<glm::vec3> hitbox;
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

        // Calculate hitbox

        glm::vec3 min = modelData->vertices[0];
        glm::vec3 max = modelData->vertices[0];

        for (auto vertex : modelData->vertices) {
            if (vertex.x < min.x) {
                min.x = vertex.x;
            }
            if (vertex.y < min.y) {
                min.y = vertex.y;
            }
            if (vertex.z < min.z) {
                min.z = vertex.z;
            }
            if (vertex.x > max.x) {
                max.x = vertex.x;
            }
            if (vertex.y > max.y) {
                max.y = vertex.y;
            }
            if (vertex.z > max.z) {
                max.z = vertex.z;
            }
        }

        modelData->hitbox = {min, max};

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

    static void normalizeObjectSize(ModelData *modelData);
};

void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint>& indices,
                    int subdivisions);

#endif //HOMEWORK1_RESOURCEMANAGER_H
