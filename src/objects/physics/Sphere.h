//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_SPHERE_H
#define HOMEWORK1_SPHERE_H


#include "Object.h"

class Sphere : public Object {


public:
    explicit Sphere(GLfloat scale) {
        this->scale = scale;
        this->modelType = ModelTypes::SPHERE;
    }

};


#endif //HOMEWORK1_SPHERE_H
