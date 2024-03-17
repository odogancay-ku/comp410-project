//
// Created by ofaru on 16.03.2024.
//

#include <complex>
#include <glm/geometric.hpp>
#include "ResourceManager.h"


void ResourceManager::generateBuiltinModels() {
    ModelData cubeModelData;
    cubeModelData.type = ModelTypes::CUBE;

    ModelData sphereModelData;
    sphereModelData.type = ModelTypes::SPHERE;

    ModelData cylinderModelData;
    cylinderModelData.type = ModelTypes::CYLINDER;

    ModelData coneModelData;
    coneModelData.type = ModelTypes::CONE;

    ModelData torusModelData;
    torusModelData.type = ModelTypes::TORUS;

    ModelData teapotModelData;
    teapotModelData.type = ModelTypes::TEAPOT;

    ModelData planeModelData;
    planeModelData.type = ModelTypes::PLANE;

    ModelData skyboxModelData;
    skyboxModelData.type = ModelTypes::SKYBOX;

    ModelData quadModelData;
    quadModelData.type = ModelTypes::QUAD;

    cubeModelData.vertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    cubeModelData.indices = {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
    };

    cubeModelData.normals = {
            glm::vec3(-1.0f, -1.0f, -1.0f), // Front bottom left
            glm::vec3(1.0f, -1.0f, -1.0f),  // Front bottom right
            glm::vec3(1.0f, 1.0f, -1.0f),   // Front top right
            glm::vec3(-1.0f, 1.0f, -1.0f),  // Front top left
            glm::vec3(-1.0f, -1.0f, 1.0f),  // Back bottom left
            glm::vec3(1.0f, -1.0f, 1.0f),   // Back bottom right
            glm::vec3(1.0f, 1.0f, 1.0f),    // Back top right
            glm::vec3(-1.0f, 1.0f, 1.0f)    // Back top left
    };

    cubeModelData.colorVertices = {
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

    generateSphere(sphereModelData.vertices, sphereModelData.normals, sphereModelData.indices, 3);

    std::cout << "Sphere vertices: " << sphereModelData.vertices.size() << std::endl;
    std::cout << "Sphere normals: " << sphereModelData.normals.size() << std::endl;
    std::cout << "Sphere indices: " << sphereModelData.indices.size() << std::endl;

    for (int i = 0; i < sphereModelData.vertices.size(); ++i) {
        sphereModelData.colorVertices.emplace_back(1.0f, 0.0f, 0.0f);
    }

    ResourceManager::addModel(ModelTypes::SPHERE, sphereModelData);


}

void ResourceManager::generateExternalModels() {

    ModelData bunnyModelData;

    ResourceManager::loadModel("assets/models/bunny.off", ModelTypes::BUNNY);

    ModelData maidModelData;

    ResourceManager::loadModel("assets/models/maid.off", ModelTypes::MAID);

}



void divideTriangle(std::vector<glm::vec3>& vertices, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, int depth) {
    if (depth == 0) {
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    } else {
        // Calculate midpoints
        glm::vec3 ab = glm::normalize(glm::vec3 {(a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2});
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
void generateSphere(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices, int subdivisions) {
    // Initial vertices of a tetrahedron
    glm::vec3 v0 = glm::normalize(glm::vec3 {1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v1 = glm::normalize(glm::vec3 {-1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v2 = glm::normalize(glm::vec3 {0.0f, 1.0f, 1.0f / std::sqrt(2.0f)});
    glm::vec3 v3 = glm::normalize(glm::vec3 {0.0f, -1.0f, 1.0f / std::sqrt(2.0f)});

    // Subdivide each face of the tetrahedron
    divideTriangle(vertices, v0, v2, v3, subdivisions);
    divideTriangle(vertices, v1, v3, v2, subdivisions);
    divideTriangle(vertices, v0, v3, v1, subdivisions);
    divideTriangle(vertices, v0, v1, v2, subdivisions);

    // Calculate normals
    normals.resize(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        normals[i] = normalize(vertices[i]);

    }

    // Generate indices
    for (size_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(static_cast<GLuint>(i));
    }
}