#include "RubiksCube.h"

#include <iostream>

void RubiksCube::addCubes() {

    int blueIndices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int greenIndices[9] = {18, 19, 20, 21, 22, 23, 24, 25, 26};
    int redIndices[9] = {0, 3, 6, 9, 12, 15, 18, 21, 24};
    int orangeIndices[9] = {2, 5, 8, 11, 14, 17, 20, 23, 26};
    int yellowIndices[9] = {6, 7, 8, 15, 16, 17, 24, 25, 26};
    int whiteIndices[9] = {0, 1, 2, 9, 10, 11, 18, 19, 20};

    for (int i = 0; i < 27; i++) {
        std::cout << "Adding cube " << i << std::endl;

        cubes[i] = new Object();
        cubes[i]->position = glm::vec3(i % 3 + position.x -1.0f, ((i / 3) % 3) + position.y -1.0f, i / 9 + position.z-1.0f);

        /**
         *
         *
         *
         */

        cubes[i]->canMove = false;
        cubes[i]->canRotate = true;
        cubes[i]->canCollide = true;
        cubes[i]->applyPhysics = false;


        std::cout << "Setting model data" << std::endl;


        auto *modelData = new ModelData();
        modelData->type = ModelTypes::UNIQUE_MODEL;
        modelData->material = ResourceManager::whitePlastic;

        modelData->vertices = {

                // Left face
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, -0.5f),

                // Left face 2
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, -0.5f),

                // Right face
                glm::vec3(-0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),

                // Right face 2
                glm::vec3(-0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f),

                // Top face
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),

                // Top face 2
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f),

                // Bottom face
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),

                // Bottom face 2
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f),

                // Front face
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f),

                // Front face 2
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f),

                // Back face
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),

                // Back face 2
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f)

        };

        modelData->colorVertices = std::vector<glm::vec3>(modelData->vertices.size(), glm::vec3(1.0f, 0.0f, 0.0f));

        modelData->indices = {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                27, 28, 29, 30, 31, 32, 33, 34, 35
        };


        for (glm::vec3 &vertex: modelData->vertices) {
            modelData->normals.push_back(glm::normalize(vertex));
        }

        for (int j = 0; j < modelData->colorVertices.size(); j++) {

            // Left face blue
            // Front face red
            // Top face green
            // Right face orange
            // Bottom face yellow
            // Back face white

            if (j < 6) {
                modelData->colorVertices[j] = glm::vec3(0.0f, 0.0f, 1.0f);
            } else if (j < 12) {
                modelData->colorVertices[j] = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (j < 18) {
                modelData->colorVertices[j] = glm::vec3(0.0f, 1.0f, 0.0f);
            } else if (j < 24) {
                modelData->colorVertices[j] = glm::vec3(1.0f, 0.5f, 0.0f);
            } else if (j < 30) {
                modelData->colorVertices[j] = glm::vec3(1.0f, 1.0f, 0.0f);
            } else {
                modelData->colorVertices[j] = glm::vec3(1.0f, 1.0f, 1.0f);
            }

        }

        ResourceManager::setModelHitbox(modelData);

        cubes[i]->modelType = ModelTypes::UNIQUE_MODEL;
        cubes[i]->modelData = modelData;

    }

}

glm::vec3
rotateVectorAroundAxisThroughPoint(glm::vec3 point, glm::vec3 axis, float angleDegrees, glm::vec3 rotationCenter) {
    // Step 1: Translate the rotation center to the origin
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -rotationCenter);

    // Step 2: Rotate around the axis
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleDegrees), axis);

    // Step 3: Translate back to the original center
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), rotationCenter);

    // Combine the transformations
    glm::mat4 transform = translateBack * rotation * translateToOrigin;

    // Apply the transformation to the point
    glm::vec4 transformedPoint = transform * glm::vec4(point, 1.0f);

    return glm::vec3(transformedPoint);
}

void RubiksCube::rotateRow(int row, float angle) {

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    for (auto &cube: cubes) {

        float distanceToRow1 = abs(cube->position.y - row1Y);
        float distanceToRow2 = abs(cube->position.y - row2Y);
        float distanceToRow3 = abs(cube->position.y - row3Y);

        int closestRow = 0;

        if (distanceToRow2 < distanceToRow1) {
            closestRow = 1;
        }

        if (distanceToRow3 < distanceToRow2) {
            closestRow = 2;
        }

        if (closestRow == row) {

            glm::vec3 rotationCenter = glm::vec3(position.x, cube->position.y, position.z);

            cube->position = rotateVectorAroundAxisThroughPoint(cube->position, glm::vec3(0.0f, 1.0f, 0.0f), angle,
                                                                rotationCenter);
            cube->rotation.y += angle;
        }
    }

}

RubiksCube::RubiksCube(const glm::vec3 &position) : position(position) {
    row1Y = position.y - 1.0f;
    row2Y = position.y;
    row3Y = position.y + 1.0f;

    col1X = position.x - 1.0f;
    col2X = position.x;
    col3X = position.x + 1.0f;

    col1Z = position.z - 1.0f;
    col2Z = position.z;
    col3Z = position.z + 1.0f;
}

