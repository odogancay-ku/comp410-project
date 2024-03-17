//
// Created by ofaru on 16.03.2024.
//

#include <complex>
#include <glm/geometric.hpp>
#include "ResourceManager.h"


void ResourceManager::generateBuiltinModels() {
    ModelData* cubeModelData = new ModelData();
    cubeModelData->type = ModelTypes::CUBE;

    ModelData* sphereModelData = new ModelData();
    sphereModelData->type = ModelTypes::SPHERE;

    ModelData* cylinderModelData = new ModelData();
    cylinderModelData->type = ModelTypes::CYLINDER;

    ModelData* coneModelData = new ModelData();
    coneModelData->type = ModelTypes::CONE;

    ModelData* torusModelData = new ModelData();
    torusModelData->type = ModelTypes::TORUS;

    ModelData* teapotModelData = new ModelData();
    teapotModelData->type = ModelTypes::TEAPOT;

    ModelData* planeModelData = new ModelData();
    planeModelData->type = ModelTypes::PLANE;

    ModelData* skyboxModelData = new ModelData();
    skyboxModelData->type = ModelTypes::SKYBOX;

    ModelData* quadModelData = new ModelData();
    quadModelData->type = ModelTypes::QUAD;

    cubeModelData->vertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    cubeModelData->indices = {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
    };

    cubeModelData->normals = {
            glm::vec3(-1.0f, -1.0f, -1.0f), // Front bottom left
            glm::vec3(1.0f, -1.0f, -1.0f),  // Front bottom right
            glm::vec3(1.0f, 1.0f, -1.0f),   // Front top right
            glm::vec3(-1.0f, 1.0f, -1.0f),  // Front top left
            glm::vec3(-1.0f, -1.0f, 1.0f),  // Back bottom left
            glm::vec3(1.0f, -1.0f, 1.0f),   // Back bottom right
            glm::vec3(1.0f, 1.0f, 1.0f),    // Back top right
            glm::vec3(-1.0f, 1.0f, 1.0f)    // Back top left
    };

    cubeModelData->colorVertices = {
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
    };

    ResourceManager::addModel(ModelTypes::CUBE, cubeModelData);

    generateSphere(sphereModelData->vertices, sphereModelData->normals, sphereModelData->indices, 3);

    std::cout << "Sphere vertices: " << sphereModelData->vertices.size() << std::endl;
    std::cout << "Sphere normals: " << sphereModelData->normals.size() << std::endl;
    std::cout << "Sphere indices: " << sphereModelData->indices.size() << std::endl;

    for (int i = 0; i < sphereModelData->vertices.size(); ++i) {
        sphereModelData->colorVertices.emplace_back(1.0f, 1.0f, 1.0f);
    }

    ResourceManager::addModel(ModelTypes::SPHERE, sphereModelData);


}

void ResourceManager::generateExternalModels() {

    ModelData bunnyModelData;

    ResourceManager::loadModel("assets/models/bunny.off", ModelTypes::BUNNY);

    ModelData maidModelData;

    ResourceManager::loadModel("assets/models/maid.off", ModelTypes::MAID);

}

void ResourceManager::bufferModelData(ModelTypes modelType, ModelData* modelData) {
    // Using modelData create the necessary VBO, VAO, and EBO
    // positions, normals will be packed together, and color will be packed separately, indices will be in VAO

    std::cout << "Buffering model data for " << modelType << std::endl;


    GLuint VBO, EBO, VAO;


    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Merge vertices and normals

    std::vector<glm::vec3> verticesAndNormals = {};

    for (int i = 0; i < modelData->vertices.size(); ++i) {
        verticesAndNormals.push_back(modelData->vertices[i]);
        // If normal is not available, use the vertex as normal
        if (i < modelData->normals.size()) {
            verticesAndNormals.push_back(modelData->normals[i]);
        } else {
            verticesAndNormals.push_back(modelData->vertices[i]);
        }
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesAndNormals.size() * sizeof(glm::vec3), &verticesAndNormals[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData->indices.size() * sizeof(GLuint), &modelData->indices[0],
                 GL_STATIC_DRAW);


    // Set the vertex attribute pointers

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    modelData->VAO = VAO;
    modelData->VBO = VBO;
    modelData->EBO = EBO;

    std::cout << "Buffering model data complete for " << modelType << " " << VAO << " " << VBO << " " << EBO
              << std::endl;


}


void divideTriangle(std::vector<glm::vec3> &vertices, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c,
                    int depth) {
    if (depth == 0) {
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    } else {
        // Calculate midpoints
        glm::vec3 ab = glm::normalize(glm::vec3{(a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2});
        glm::vec3 ac = glm::normalize(glm::vec3{(a.x + c.x) / 2, (a.y + c.y) / 2, (a.z + c.z) / 2});
        glm::vec3 bc = glm::normalize(glm::vec3{(b.x + c.x) / 2, (b.y + c.y) / 2, (b.z + c.z) / 2});

        // Recursively subdivide
        divideTriangle(vertices, a, ab, ac, depth - 1);
        divideTriangle(vertices, ab, b, bc, depth - 1);
        divideTriangle(vertices, ac, bc, c, depth - 1);
        divideTriangle(vertices, ab, bc, ac, depth - 1);
    }
}

// Function to generate normals and indices
void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint> &indices,
                    int subdivisions) {
    // Initial vertices of a tetrahedron
    glm::vec3 v0 = glm::normalize(glm::vec3{1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v1 = glm::normalize(glm::vec3{-1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v2 = glm::normalize(glm::vec3{0.0f, 1.0f, 1.0f / std::sqrt(2.0f)});
    glm::vec3 v3 = glm::normalize(glm::vec3{0.0f, -1.0f, 1.0f / std::sqrt(2.0f)});

    // Subdivide each face of the tetrahedron
    divideTriangle(vertices, v0, v2, v3, subdivisions);
    divideTriangle(vertices, v1, v3, v2, subdivisions);
    divideTriangle(vertices, v0, v3, v1, subdivisions);
    divideTriangle(vertices, v0, v1, v2, subdivisions);

    // Calculate normals
    for (auto vertex: vertices) {
        normals.push_back(glm::normalize(vertex));
    }

    // Generate indices
    for (size_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(static_cast<GLuint>(i));
    }
}