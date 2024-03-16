//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_CUBE_H
#define HOMEWORK1_CUBE_H


#include <glew.h>
#include "Object.h"

class Cube : public Object {


public:
    explicit Cube(GLfloat scale) {
        this->scale = scale;
    }

};

#endif //HOMEWORK1_CUBE_H
