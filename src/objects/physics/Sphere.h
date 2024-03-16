//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_SPHERE_H
#define HOMEWORK1_SPHERE_H


#include "Object.h"

class Sphere : public Object {

    GLfloat radius;

public:
    Sphere(GLfloat radius, glm::vec3 position) {
        this->radius = radius;
        this->position = position;
        this->modelType = ModelTypes::SPHERE;
    }

};


#endif //HOMEWORK1_SPHERE_H
