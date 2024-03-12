//
// Created by ofaru on 13.03.2024.
//

#ifndef GAME_H
#define GAME_H

#include "../physics/object.h"

class Game {
public:
    std::vector<Object> objects;


    void addObject(const Object &object);

    void update(float dt);

    void checkCollisions();

    void removeObject(const Object &object);

    void draw(Renderer &renderer);

};

#endif //GAME_H
