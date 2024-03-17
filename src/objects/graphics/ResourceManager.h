//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_RESOURCEMANAGER_H
#define HOMEWORK1_RESOURCEMANAGER_H

#include <vector>
#include <glm/vec3.hpp>
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

//        bufferModelData((ModelTypes) modelIndex, modelData);


    }

    // Load model from file and store in the ResourceManager
    static void loadModel(const std::string &filePath, const int modelIndex) {

        // Load model data from file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        ModelData* modelData = new ModelData();
        std::string line;
        std::string firstLine;
        std::getline(file, firstLine);
        if (firstLine != "OFF") {
            std::cout << firstLine.data() << std::endl;
            std::cout << "ERROR: File is not in OFF format" << std::endl;
            exit(1);
        }

        // Read the number of vertices, faces, and edges

        int numVertices, numFaces, numEdges;
        file >> numVertices >> numFaces >> numEdges;


        // Read the vertices
        for (int i = 0; i < numVertices; ++i) {
            float x, y, z, cx, cy, cz;
            file >> x >> y >> z >> cx >> cy >> cz;

            modelData->vertices.emplace_back(x, y, z);
            modelData->colorVertices.emplace_back(0.8f, 0.8f, 0.8f);
        }

        // Read the faces
        for (int i = 0; i < numFaces; ++i) {
            int numSides;
            file >> numSides;
            for (int j = 0; j < numSides; ++j) {
                GLuint index;
                file >> index;
                modelData->indices.push_back(index);
            }
        }

        // Store loaded model data in the ResourceManager
        addModel(modelIndex, modelData);


    }


    static ModelData* getModel(const ModelTypes modelType) {
        return models[modelType];
    }

    static void generateExternalModels();

    void static bufferModelData(ModelTypes modelType, ModelData* modelData);

};

void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint>& indices,
                    int subdivisions);

#endif //HOMEWORK1_RESOURCEMANAGER_H
