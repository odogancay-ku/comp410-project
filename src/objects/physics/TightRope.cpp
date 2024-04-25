//
// Created by ofarukdogancay on 4/25/24.
//

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "TightRope.h"
#include "objects/graphics/ResourceManager.h"


ModelData* generateRope(glm::vec3 point1, glm::vec3 point2, float radius, int segments, glm::vec3 colorStart, glm::vec3 colorEnd) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
    std::vector<glm::vec3> colors;

    glm::vec3 axis = point2 - point1;
    float height = glm::length(axis);
    axis = glm::normalize(axis);

    // Generate perpendicular vector
    glm::vec3 up = abs(axis.y) < 0.99f ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
    glm::vec3 right = glm::normalize(glm::cross(up, axis));
    up = glm::cross(axis, right);

    // Generate circle points
    for (int i = 0; i < segments; ++i) {
        float angle = 2 * M_PI * i / segments;
        glm::vec3 radial = right * ((float) cos(angle)) + up * ((float) sin(angle));

        glm::vec3 v0;
        glm::vec3 v1;

        glm::vec3 n0;
        glm::vec3 n1;

        v0 = point1 + radius * radial;
        v1 = point2 + radius * radial;
        n0 = n1 = glm::normalize(radial);


        // Add indices for triangles
        int next = (i + 1) % segments;

        glm::vec3 color0 = glm::mix(colorStart, colorEnd, static_cast<float>(i) / (segments - 1));
        glm::vec3 color1 = glm::mix(colorStart, colorEnd, static_cast<float>(i + 1) / (segments - 1));

        vertices.push_back(v0);
        vertices.push_back(v1);
        normals.push_back(n0);
        normals.push_back(n1);
        colors.push_back(color0);
        colors.push_back(color1);

        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);

        // First triangle
        vertices.push_back(v0);
        normals.push_back(n0);
        colors.push_back(color0);

        vertices.push_back(vertices[2 * next]);
        normals.push_back(normals[2 * next]);
        colors.push_back(color1);

        vertices.push_back(v1);
        normals.push_back(n1);
        colors.push_back(color0);

        indices.push_back(vertices.size() - 3);
        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);

        // Second triangle
        vertices.push_back(vertices[2 * next]);
        normals.push_back(normals[2 * next]);
        colors.push_back(color1);

        vertices.push_back(vertices[2 * next + 1]);
        normals.push_back(normals[2 * next + 1]);
        colors.push_back(color1);

        vertices.push_back(v1);
        normals.push_back(n1);
        colors.push_back(color0);

        indices.push_back(vertices.size() - 3);
        indices.push_back(vertices.size() - 2);
        indices.push_back(vertices.size() - 1);


    }

    ModelData* modelData = new ModelData();
    modelData->vertices = vertices;
    modelData->normals = normals;
    modelData->indices = indices;
    modelData->colorVertices = colors;

    modelData->type = ModelTypes::UNIQUE_MODEL;
    modelData->material = ResourceManager::whitePlastic;

    return modelData;
}

TightRope::TightRope(glm::vec3 point1, glm::vec3 point2, float radius, int segments, glm::vec3 colorStart, glm::vec3 colorEnd) {
    this->applyPhysics = false;
    this->canCollide = false;
    this->canMove = false;
    this->canRotate = false;
    this->start = point1;
    this->end = point2;
    this->radius = radius;
    this->segments = segments;
    this->colorStart = colorStart;
    this->colorEnd = colorEnd;

    this->modelData = generateRope(point1, point2, radius, segments, colorStart, colorEnd);
    this->modelType = ModelTypes::UNIQUE_MODEL;

}

void TightRope::updateEnds(glm::vec3 point1, glm::vec3 point2) {
    this->start = point1;
    this->end = point2;
    this->modelData = generateRope(point1, point2, radius, segments, colorStart, colorEnd);
}
