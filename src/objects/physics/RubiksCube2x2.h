//
// Created by ofarukdogancay on 4/24/24.
//

#ifndef COMP410_PROJECT_RUBIKSCUBE2X2_H
#define COMP410_PROJECT_RUBIKSCUBE2X2_H

#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include "objects/graphics/ResourceManager.h"
#include "Object.h"

class RubiksCube2x2 {

public:


    explicit RubiksCube2x2(const glm::vec3 &position);

    Object* cubes[8];
    glm::vec3 position;
    float columnWidth = 1.0f;

    float rotatingColumn = -1;
    glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
    float rotationAngle = 0.0f;
    std::vector<Object*> rotatingCubes;
    std::vector<glm::vec3> originalPositions;
    std::vector<glm::quat> originalOrientations;

    void addCubes();

    std::pair<int,glm::vec3> determineRotationAxisAndColumn(Object* hitObject, glm::vec3 hitPos, glm::vec3 pullPos);

    void determineAndStartRotation(Object* hitObject, glm::vec3 hitPos, glm::vec3 pullPos);

    void updateRotation(float dt, glm::vec3 hitPos, glm::vec3 pullPos);

    void rotateCurrentColumnForAngle(float angle);

    void finishRotation();

    void setCubesOfRotation();

    void rotateColumn(int column, float angle, glm::vec3 axis);

};



#endif //COMP410_PROJECT_RUBIKSCUBE2X2_H
