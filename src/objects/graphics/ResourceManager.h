//
// Created by ofaru on 16.03.2024.
//

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

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

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

enum ModelTypes {
    CUBE,
    SPHERE,
    BUNNY,
    MAID,
    UNIQUE_MODEL,
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

    Material material;

    std::vector<glm::vec3> hitboxVertices;
    std::vector<GLuint> hitboxIndices;
    std::vector<glm::vec3> hitboxNormals;
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colorVertices;
};

// Resource Manager class to manage loading and sharing of model data
class ResourceManager {
private:
    static inline ModelData *models[ModelTypes::END_OF_TYPES_MARKER] = {};
    static inline std::vector<ModelData> uniqueModels;

public:

    static ResourceManager &getInstance() {
        static ResourceManager instance;
        return instance;
    };

    static void generateBuiltinModels();

    static void addModel(const int modelIndex, ModelData *modelData) {

        setModelHitbox(modelData);

        models[modelIndex] = modelData;

        std::cout << "Model added: " << modelIndex << std::endl;
        std::cout << "Model added: " << models[modelIndex]->type << std::endl;
        std::cout << "Model added: " << models[modelIndex]->indices.size() << std::endl;

//        bufferModelData((ModelTypes) modelIndex, modelData);


    }

    static void setModelHitbox(ModelData* modelData) {
        // Calculate hitbox

        glm::vec3 min = modelData->vertices[0];
        glm::vec3 max = modelData->vertices[0];

        for (auto vertex: modelData->vertices) {
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

        modelData->hitboxVertices = {
                glm::vec3(min.x, min.y, min.z),
                glm::vec3(max.x, min.y, min.z),
                glm::vec3(max.x, max.y, min.z),
                glm::vec3(min.x, max.y, min.z),
                glm::vec3(min.x, min.y, max.z),
                glm::vec3(max.x, min.y, max.z),
                glm::vec3(max.x, max.y, max.z),
                glm::vec3(min.x, max.y, max.z)
        };

        modelData->hitboxIndices = {
                0, 1, 2, 2, 3, 0,
                1, 5, 6, 6, 2, 1,
                7, 6, 5, 5, 4, 7,
                4, 0, 3, 3, 7, 4,
                3, 2, 6, 6, 7, 3,
                4, 5, 1, 1, 0, 4
        };

        // Initialize all normals to zero
        modelData->hitboxNormals = std::vector<glm::vec3>(modelData->hitboxVertices.size(),
                                                          glm::vec3(0.0f, 0.0f, 0.0f));

        // Calculate the normals for each face
        for (int i = 0; i < modelData->hitboxIndices.size(); i += 3) {
            glm::vec3 v0 = modelData->hitboxVertices[modelData->hitboxIndices[i]];
            glm::vec3 v1 = modelData->hitboxVertices[modelData->hitboxIndices[i + 1]];
            glm::vec3 v2 = modelData->hitboxVertices[modelData->hitboxIndices[i + 2]];

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;

            glm::vec3 faceNormal = glm::cross(edge1, edge2);

            // Add the face's normal to each of the vertices' normals
            modelData->hitboxNormals[modelData->hitboxIndices[i]] += faceNormal;
            modelData->hitboxNormals[modelData->hitboxIndices[i + 1]] += faceNormal;
            modelData->hitboxNormals[modelData->hitboxIndices[i + 2]] += faceNormal;
        }

        // Normalize each vertex normal
        for (auto &normal: modelData->hitboxNormals) {
            normal = glm::normalize(normal);
        }
    }

    static ModelData *getModel(const ModelTypes modelType) {
        return models[modelType];
    }

    static void generateExternalModels();

    void static bufferModelData(ModelTypes modelType, ModelData *modelData);

    static void loadModel(const std::string &filePath, ModelData *modelIndex);

    static void normalizeObjectSize(ModelData *modelData);

    // MATERIALS

    /**
Name 	Ambient 	Diffuse 	Specular 	Shininess
emerald 	0.0215 	0.1745 	0.0215 	0.07568 	0.61424 	0.07568 	0.633 	0.727811 	0.633 	0.6
jade 	0.135 	0.2225 	0.1575 	0.54 	0.89 	0.63 	0.316228 	0.316228 	0.316228 	0.1
obsidian 	0.05375 	0.05 	0.06625 	0.18275 	0.17 	0.22525 	0.332741 	0.328634 	0.346435 	0.3
pearl 	0.25 	0.20725 	0.20725 	1 	0.829 	0.829 	0.296648 	0.296648 	0.296648 	0.088
ruby 	0.1745 	0.01175 	0.01175 	0.61424 	0.04136 	0.04136 	0.727811 	0.626959 	0.626959 	0.6
turquoise 	0.1 	0.18725 	0.1745 	0.396 	0.74151 	0.69102 	0.297254 	0.30829 	0.306678 	0.1
brass 	0.329412 	0.223529 	0.027451 	0.780392 	0.568627 	0.113725 	0.992157 	0.941176 	0.807843 	0.21794872
bronze 	0.2125 	0.1275 	0.054 	0.714 	0.4284 	0.18144 	0.393548 	0.271906 	0.166721 	0.2
chrome 	0.25 	0.25 	0.25 	0.4 	0.4 	0.4 	0.774597 	0.774597 	0.774597 	0.6
copper 	0.19125 	0.0735 	0.0225 	0.7038 	0.27048 	0.0828 	0.256777 	0.137622 	0.086014 	0.1
gold 	0.24725 	0.1995 	0.0745 	0.75164 	0.60648 	0.22648 	0.628281 	0.555802 	0.366065 	0.4
silver 	0.19225 	0.19225 	0.19225 	0.50754 	0.50754 	0.50754 	0.508273 	0.508273 	0.508273 	0.4
black plastic 	0.0 	0.0 	0.0 	0.01 	0.01 	0.01 	0.50 	0.50 	0.50 	.25
cyan plastic 	0.0 	0.1 	0.06 	0.0 	0.50980392 	0.50980392 	0.50196078 	0.50196078 	0.50196078 	.25
green plastic 	0.0 	0.0 	0.0 	0.1 	0.35 	0.1 	0.45 	0.55 	0.45 	.25
red plastic 	0.0 	0.0 	0.0 	0.5 	0.0 	0.0 	0.7 	0.6 	0.6 	.25
white plastic 	0.0 	0.0 	0.0 	0.55 	0.55 	0.55 	0.70 	0.70 	0.70 	.25
yellow plastic 	0.0 	0.0 	0.0 	0.5 	0.5 	0.0 	0.60 	0.60 	0.50 	.25
black rubber 	0.02 	0.02 	0.02 	0.01 	0.01 	0.01 	0.4 	0.4 	0.4 	.078125
cyan rubber 	0.0 	0.05 	0.05 	0.4 	0.5 	0.5 	0.04 	0.7 	0.7 	.078125
green rubber 	0.0 	0.05 	0.0 	0.4 	0.5 	0.4 	0.04 	0.7 	0.04 	.078125
red rubber 	0.05 	0.0 	0.0 	0.5 	0.4 	0.4 	0.7 	0.04 	0.04 	.078125
white rubber 	0.05 	0.05 	0.05 	0.5 	0.5 	0.5 	0.7 	0.7 	0.7 	.078125
yellow rubber 	0.05 	0.05 	0.0 	0.5 	0.5 	0.4 	0.7 	0.7 	0.04 	.078125
     */

    static Material defaultMaterial;
    static Material emerald;
    static Material jade;
    static Material obsidian;
    static Material pearl;
    static Material ruby;
    static Material turquoise;
    static Material brass;
    static Material bronze;
    static Material chrome;
    static Material copper;
    static Material gold;
    static Material silver;
    static Material blackPlastic;
    static Material cyanPlastic;
    static Material greenPlastic;
    static Material redPlastic;
    static Material whitePlastic;
    static Material yellowPlastic;
    static Material blackRubber;
    static Material cyanRubber;
    static Material greenRubber;
    static Material redRubber;
    static Material whiteRubber;
    static Material yellowRubber;


};

void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint> &indices,
                    int subdivisions);

#endif //RESOURCEMANAGER_H
