//
// Created by ofarukdogancay on 4/24/24.
//

#include "RubiksCube2x2.h"


#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

void RubiksCube2x2::addCubes() {


    for (int i = 0; i < 8; i++) {
        cubes[i] = new Object();
        cubes[i]->position = glm::vec3(i % 2 + position.x - 0.5f, ((i / 2) % 2) + position.y - 0.5f,
                                       i / 4 + position.z - 0.5f);

        /**
         *
         *
         *
         */

        cubes[i]->canMove = false;
        cubes[i]->canRotate = true;
        cubes[i]->canCollide = true;
        cubes[i]->applyPhysics = false;

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

RubiksCube2x2::RubiksCube2x2(const glm::vec3 &position) : position(position) {
}

std::pair<int, glm::vec3>
RubiksCube2x2::determineRotationAxisAndColumn(Object *hitObject, glm::vec3 hitPos, glm::vec3 pullPos) {


    glm::vec3 displacement = pullPos - hitPos;

    float positionOnAxisX = glm::dot((hitObject->position - position), glm::vec3(1.0f, 0.0f, 0.0f));
    float positionOnAxisY = glm::dot((hitObject->position - position), glm::vec3(0.0f, 1.0f, 0.0f));
    float positionOnAxisZ = glm::dot((hitObject->position - position), glm::vec3(0.0f, 0.0f, 1.0f));

    int closestColX = 1;

    if (positionOnAxisX < 0) {
        closestColX = 0;
    }

    int closestColY = 1;

    if (positionOnAxisY < 0) {
        closestColY = 0;
    }

    int closestColZ = 1;

    if (positionOnAxisZ < 0) {
        closestColZ = 0;
    }

    glm::vec3 centerOfRotationOnXAxisRotation = glm::vec3(closestColX - 0.5, 0.0f, 0.0f);
    glm::vec3 centerOfRotationOnYAxisRotation = glm::vec3(0.0f, closestColY - 0.5, 0.0f);
    glm::vec3 centerOfRotationOnZAxisRotation = glm::vec3(0.0f, 0.0f, closestColZ - 0.5);

    float torqueX = glm::length(glm::cross(displacement, centerOfRotationOnXAxisRotation));
    float torqueY = glm::length(glm::cross(displacement, centerOfRotationOnYAxisRotation));
    float torqueZ = glm::length(glm::cross(displacement, centerOfRotationOnZAxisRotation));

    if (torqueX > torqueY && torqueX > torqueZ) {
        return {closestColX, glm::vec3(1.0f, 0.0f, 0.0f)};
    } else if (torqueY > torqueX && torqueY > torqueZ) {
        return {closestColY, glm::vec3(0.0f, 1.0f, 0.0f)};
    } else {
        return {closestColZ, glm::vec3(0.0f, 0.0f, 1.0f)};
    }

}

void RubiksCube2x2::determineAndStartRotation(Object *hitObject, glm::vec3 hitPos, glm::vec3 pullPos) {

    if (rotatingColumn != -1) {
        return;
    }

    if (glm::length(pullPos - hitPos) < 0.1f) {
        return;
    }

    std::pair<int, glm::vec3> rotationAxisAndColumn = determineRotationAxisAndColumn(hitObject, hitPos, pullPos);

    rotatingColumn = rotationAxisAndColumn.first;
    rotationAxis = rotationAxisAndColumn.second;

    setCubesOfRotation();

}

void RubiksCube2x2::updateRotation(float dt, glm::vec3 hitPos, glm::vec3 pullPos) {

    if (rotatingColumn == -1) {
        return;
    }

    glm::vec3 centerOfRotation =
            glm::vec3(rotatingColumn - 0.5, rotatingColumn - 0.5, rotatingColumn - 0.5) * rotationAxis + position;


    glm::vec3 pullOnAxis = (pullPos - centerOfRotation) * (glm::vec3(1.0f) - rotationAxis);
    glm::vec3 hitOnAxis = (hitPos - centerOfRotation) * (glm::vec3(1.0f) - rotationAxis);

    float angleDiff = glm::degrees(glm::angle(glm::normalize(pullOnAxis), glm::normalize(hitOnAxis)));

    glm::vec3 crossProduct = glm::cross(pullOnAxis, hitOnAxis);
    if (glm::dot(crossProduct, rotationAxis) > 0) {
        angleDiff = -angleDiff;
    }

    rotateCurrentColumnForAngle(angleDiff);

}

void RubiksCube2x2::finishRotation() {

    if (rotatingColumn == -1) {
        return;
    }

    float roundedAngle = round(rotationAngle / 90.0f) * 90.0f;

    rotateCurrentColumnForAngle(roundedAngle-rotationAngle);

    rotatingColumn = -1;
    rotatingCubes.clear();

}

void RubiksCube2x2::rotateCurrentColumnForAngle(float angle) {

    if (rotatingColumn == -1) {
        return;
    }

    glm::vec3 centerOfRotation =
            glm::vec3(rotatingColumn - 0.5, rotatingColumn - 0.5, rotatingColumn - 0.5) * rotationAxis + position;

    for (int i = 0; i < 4; i++) {

        rotatingCubes.at(i)->rotateAroundPointAndAxis(centerOfRotation, rotationAxis, glm::radians(angle));

    }

    rotationAngle += angle;


}

void RubiksCube2x2::rotateColumn(int column, float angle, glm::vec3 axis) {


    if (rotatingColumn != column) {
        rotatingColumn = column;
        rotationAxis = axis;
        rotationAngle = 0.0f;

        setCubesOfRotation();

    }

    rotateCurrentColumnForAngle(angle);

}

void RubiksCube2x2::setCubesOfRotation() {

    rotatingCubes.clear();
    originalPositions.clear();
    originalOrientations.clear();

    for (int i = 0; i < 8; i++) {

        float positionOnAxis = glm::dot((cubes[i]->position - position), rotationAxis);

        int column = 1;

        if (positionOnAxis < 0) {
            column = 0;
        }

        if (column == rotatingColumn) {
            rotatingCubes.push_back(cubes[i]);
            originalPositions.push_back(cubes[i]->position);
            originalOrientations.push_back(cubes[i]->orientation);
        }

    }

    if (rotatingCubes.size() != 4) {
        exit(1);
    }

}







