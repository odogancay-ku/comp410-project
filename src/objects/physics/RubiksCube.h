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

    void addCubes();

    void rotateCurrentColumn(float angle);



    int rotationColumn = -1;
    float rotationAngle = 0.0f;
    float hitPointStartRotationAngle = 0.0f;
    glm::vec3 rotationAxis = glm::vec3(0,0,0);

    std::vector<Object*> rotatingCubes;
    std::vector<std::pair<glm::vec3,glm::vec3>> originalPositionsAndRotations;

    std::pair<int,glm::vec3> determineRotationAxisAndColumn(Object* hitObject, glm::vec3 pos1, glm::vec3 pos2);

    void determineAndStartRotation(Object* hitObject, glm::vec3 hitPoint, glm::vec3 pullPoint);

    void startRotationOnColumn(int column, glm::vec3 axis);

    void updateRotation(float dt, Object* hitObject, glm::vec3 hitPoint, glm::vec3 pullPoint);

    void finishRotation();

};


#endif //RUBIKSCUBE_H
