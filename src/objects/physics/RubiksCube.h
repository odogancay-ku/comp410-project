#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include "objects/graphics/ResourceManager.h"
#include "Object.h"

class RubiksCube {

public:


    explicit RubiksCube(const glm::vec3 &position);

    Object* cubes[27];
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


#endif //RUBIKSCUBE_H
