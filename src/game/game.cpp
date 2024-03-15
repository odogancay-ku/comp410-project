//
// Created by ofaru on 13.03.2024.
//

#include <vector>
#include "game.h"

void Game::addObject(const Object &object) {
    objects.push_back(object);
    objectsByTypes[object.type].push_back(object);
}

void Game::draw(Renderer &renderer) {
    renderer.drawObjects(objects);
}

void Game::removeObject(const Object &object) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i].id == object.id) {
            objects.erase(objects.begin() + i);
            return;
        }
    }
}

void Game::update(float dt) {
    for (auto &object: objects) {
        object.update(dt);
    }
}

void Game::checkCollisions() {
    for (int i = 0; i < objects.size(); i++) {

        if (!objects[i].isColliding) {
            continue;
        }


        for (int j = i + 1; j < objects.size(); j++) {


            if (!objects[j].isColliding) {
                continue;
            }

            if (objects[i].isStatic && objects[j].isStatic) {
                continue;
            }


            if (objects[i].checkCollision(objects[j])) {
                vec3 collisionPoint = objects[i].calculateCollisionPoint(objects[j]);
                objects[i].handleCollision(objects[j], collisionPoint);
            }
        }
    }
}

Game::Game() {
}
