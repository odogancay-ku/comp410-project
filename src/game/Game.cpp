//
// Created by ofaru on 16.03.2024.
//

#include <algorithm>
#include "Game.h"
#include "../camera/Camera.h"
#include "persistent/level/HW1.h"

Game* Game::instance = nullptr;


void Game::pokeObjects() {


}

void Game::update(GLfloat dt) {


    Camera *camera = Camera::getActiveInstance();

    camera->update(dt);

    glm::vec3 position = camera->position;

    for (auto &pair: currentLevel->objects) {

        for (auto object: pair.second) {
            auto oldType = pair.first;
            object->update(dt, currentLevel->environment);
            if (object->modelType != oldType) {
                pair.second.erase(
                        std::remove_if(
                                pair.second.begin(),
                                pair.second.end(),
                                //here comes the C++11 lambda:
                                [&](Object* object1) {
                                    return object == object1;
                                }
                        ),
                        pair.second.end()
                );

                currentLevel->objects[object->modelType].push_back(object);

            }
        }

    }


}

void Game::setupLevels() {
    std::cout << "Setting up levels" << std::endl;
    std::shared_ptr<HW1> level = std::make_shared<HW1>();
    std::cout << "Level HW1 created" << std::endl;

    currentLevel = std::dynamic_pointer_cast<Level>(level);

    std::cout << "Levels created" << std::endl;

    levels.push_back(std::dynamic_pointer_cast<Level>(level));

    level->setup();

    std::cout << "Levels setup finished" << std::endl;
}

void Game::checkCollisions() {

    std::vector<Object*> allObjects;

    for (auto &pair: currentLevel->objects) {
        for (auto object: pair.second) {
            allObjects.push_back(object);
        }
    }

    for (auto object1: allObjects) {
        for (auto object2: allObjects) {

            if (object1 != object2 && object1->canCollide && object2->canCollide && (object1->canMove || object2->canMove)) {
               object1->checkCollision(object2);

            }
        }
    }

}

void Game::draw() {

    Renderer *renderer = Renderer::getActiveInstance();

    renderer->createAndSetViewMatrix();

    // Group objects with the same model and draw them


    for (auto &pair: currentLevel->objects) {

        renderer->drawInstancesOfModel(pair.first, &pair.second);

        if (this->drawHitboxes) {
            renderer->drawInstancesOfModel(pair.first, &pair.second, true);
        }

    }


}

void Game::addRandomCube() {

}

void Game::addRandomBunny() {

}

void Game::addRandomSphere() {

}

