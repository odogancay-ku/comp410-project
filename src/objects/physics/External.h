//
// Created by ofaru on 17.03.2024.
//

#ifndef HOMEWORK1_EXTERNAL_H
#define HOMEWORK1_EXTERNAL_H

#include "Object.h"

class External : public Object {

public:
    explicit External(ModelTypes type, GLfloat scale) {
        this->modelType = type;
        this->scale = scale;
    }

};

#endif //HOMEWORK1_EXTERNAL_H
