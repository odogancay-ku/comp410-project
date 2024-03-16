//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_PLAYER_H
#define HOMEWORK1_PLAYER_H


#include "Object.h"

class Player : public Object {

public:
    Player() {
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->modelType = ModelTypes::MAID;
    }

};


#endif //HOMEWORK1_PLAYER_H
