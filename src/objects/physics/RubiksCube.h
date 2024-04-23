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
    float row1Y;
    float row2Y;
    float row3Y;

    float col1X;
    float col2X;
    float col3X;

    float col1Z;
    float col2Z;
    float col3Z;

    void addCubes();

    void rotateColumnX(int column, float angle);

    void rotateColumnZ(int column, float angle);

    void rotateRow(int row, float angle);



};


#endif //RUBIKSCUBE_H
