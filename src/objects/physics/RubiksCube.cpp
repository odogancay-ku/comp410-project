#include "RubiksCube.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

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
        cubes[i]->position = glm::vec3(i % 3 + position.x - 1.0f, ((i / 3) % 3) + position.y - 1.0f,
                                       i / 9 + position.z - 1.0f);

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


RubiksCube::RubiksCube(const glm::vec3 &position) : position(position) {
}

std::pair<int, glm::vec3>
RubiksCube::determineRotationAxisAndColumn(Object *hitObject, glm::vec3 hitPos, glm::vec3 pullPos) {


    glm::vec3 displacement = pullPos - hitPos;

    float positionOnAxisX = glm::dot((hitObject->position - position), glm::vec3(1.0f, 0.0f, 0.0f));
    float positionOnAxisY = glm::dot((hitObject->position - position), glm::vec3(0.0f, 1.0f, 0.0f));
    float positionOnAxisZ = glm::dot((hitObject->position - position), glm::vec3(0.0f, 0.0f, 1.0f));

    int closestColX = 1;

    if (positionOnAxisX < 0) {
        closestColX = 0;
    } else if (positionOnAxisX > 0) {
        closestColX = 2;
    }

    int closestColY = 1;

    if (positionOnAxisY < 0) {
        closestColY = 0;
    } else if (positionOnAxisY > 0) {
        closestColY = 2;
    }

    int closestColZ = 1;

    if (positionOnAxisZ < 0) {
        closestColZ = 0;
    } else if (positionOnAxisZ > 0) {
        closestColZ = 2;
    }

    glm::vec3 centerOfRotationOnXAxisRotation = glm::vec3(closestColX - 1, 0.0f, 0.0f);
    glm::vec3 centerOfRotationOnYAxisRotation = glm::vec3(0.0f, closestColY - 1, 0.0f);
    glm::vec3 centerOfRotationOnZAxisRotation = glm::vec3(0.0f, 0.0f, closestColZ - 1);

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

void RubiksCube::determineAndStartRotation(Object *hitObject, glm::vec3 hitPos, glm::vec3 pullPos) {

    if (rotatingColumn != -1) {
        return;
    }

    if (glm::length(pullPos - hitPos) < 0.1f) {
        return;
    }

    std::pair<int, glm::vec3> rotationAxisAndColumn = determineRotationAxisAndColumn(hitObject, hitPos, pullPos);

    rotatingColumn = rotationAxisAndColumn.first;
    rotationAxis = rotationAxisAndColumn.second;

    std::cout << "Rotation axis: " << glm::to_string(rotationAxis) << std::endl;
    std::cout << "Rotating column: " << rotatingColumn << std::endl;

    setCubesOfRotation();

}

void RubiksCube::updateRotation(float dt, glm::vec3 hitPos, glm::vec3 pullPos) {

    if (rotatingColumn == -1) {
        return;
    }

    glm::vec3 centerOfRotation =
            glm::vec3(rotatingColumn - 1, rotatingColumn - 1, rotatingColumn - 1) * rotationAxis + position;


    glm::vec3 pullOnAxis = (pullPos - centerOfRotation) * (glm::vec3(1.0f) - rotationAxis);
    glm::vec3 hitOnAxis = (hitPos - centerOfRotation) * (glm::vec3(1.0f) - rotationAxis);

//    std::cout << "Pull on axis: " << glm::to_string(pullOnAxis) << std::endl;
//    std::cout << "Hit on axis: " << glm::to_string(hitOnAxis) << std::endl;

    float angleDiff = glm::degrees(glm::angle(glm::normalize(pullOnAxis), glm::normalize(hitOnAxis)));

    glm::vec3 crossProduct = glm::cross(pullOnAxis, hitOnAxis);
    if (glm::dot(crossProduct, rotationAxis) > 0) {
        angleDiff = -angleDiff;
    }

    rotationAngle += angleDiff;

//    std::cout << "Rotation angle: " << rotationAngle << std::endl;
//    std::cout << "Rotation axis: " << glm::to_string(rotationAxis) << std::endl;
//    std::cout << "Center of rotation: " << glm::to_string(centerOfRotation) << std::endl;
//    std::cout << "Hit position: " << glm::to_string(hitPos) << std::endl;
//    std::cout << "Pull position: " << glm::to_string(pullPos) << std::endl;
//    std::cout << "Angle diff " << angleDiff << std::endl;

    rotateCurrentColumnForAngle(rotationAngle);


}

void RubiksCube::finishRotation() {

    if (rotatingColumn == -1) {
        return;
    }

    float roundedAngle = round(rotationAngle / 90.0f) * 90.0f;

    rotateCurrentColumnForAngle(roundedAngle-rotationAngle);

    rotatingColumn = -1;
    rotatingCubes.clear();

}

void RubiksCube::rotateCurrentColumnForAngle(float angle) {

    if (rotatingColumn == -1) {
        return;
    }

    glm::vec3 centerOfRotation =
            glm::vec3(rotatingColumn - 1, rotatingColumn - 1, rotatingColumn - 1) * rotationAxis + position;

    for (int i = 0; i < 9; i++) {

//        rotatingCubes.at(i)->orientation = glm::normalize(glm::rotate(rotatingCubes.at(i)->orientation, glm::radians(angle),
//                                                       rotationAxis));
//        rotatingCubes.at(i)->position = rotateVectorAroundAxisThroughPoint(originalPositions.at(i), rotationAxis,
//                                                                           angle, centerOfRotation);


        rotatingCubes.at(i)->rotateAroundPointAndAxis(centerOfRotation, rotationAxis, glm::radians(angle));

    }

    rotationAngle += angle;


}

void RubiksCube::rotateColumn(int column, float angle, glm::vec3 axis) {


    if (rotatingColumn != column) {
        rotatingColumn = column;
        rotationAxis = axis;
        rotationAngle = 0.0f;

        setCubesOfRotation();

    }

    rotateCurrentColumnForAngle(angle);

}

void RubiksCube::setCubesOfRotation() {

    rotatingCubes.clear();
    originalPositions.clear();
    originalOrientations.clear();

    for (int i = 0; i < 27; i++) {

        float positionOnAxis = glm::dot((cubes[i]->position - position), rotationAxis);

        std::cout << "Cube " << i << " position: " << glm::to_string(cubes[i]->position) << std::endl;
        std::cout << "Position on axis: " << positionOnAxis << std::endl;

        int column = 1;

        if (positionOnAxis < -1.0f / 2 * columnWidth) {
            column = 0;
        } else if (positionOnAxis > 1.0f / 2 * columnWidth) {
            column = 2;
        }

        if (column == rotatingColumn) {
            std::cout << "Adding cube to rotating cubes" << std::endl;
            rotatingCubes.push_back(cubes[i]);
            originalPositions.push_back(cubes[i]->position);
            originalOrientations.push_back(cubes[i]->orientation);
        }

    }

    if (rotatingCubes.size() != 9) {
        std::cout << "Error: rotatingCubes size is not 9" << std::endl;
        exit(1);
    }

}







