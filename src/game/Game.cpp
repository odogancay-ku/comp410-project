//
// Created by ofaru on 16.03.2024.
//

#include <algorithm>
#include "Game.h"
#include "camera/Camera.h"
#include "persistent/level/HW1.h"
#include "game/persistent/level/HW2.h"
#include "game/persistent/level/HW3.h"

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

    currentLevel->onUpdate(dt);

}

void Game::setupLevels() {
    std::cout << "Setting up levels" << std::endl;
    std::shared_ptr<HW1> level = std::make_shared<HW1>();
    std::shared_ptr<HW2> level2 = std::make_shared<HW2>();
    std::shared_ptr<HW3> level3 = std::make_shared<HW3>();
    std::cout << "Level HW1 created" << std::endl;

    currentLevel = std::dynamic_pointer_cast<Level>(level3);

    std::cout << "Levels created" << std::endl;

    levels.push_back(std::dynamic_pointer_cast<Level>(level));
    levels.push_back(std::dynamic_pointer_cast<Level>(level2));
    levels.push_back(std::dynamic_pointer_cast<Level>(level3));

    level3->setup();

    std::cout << "Levels setup finished" << std::endl;
}

void Game::checkCollisions() {

    std::vector<Object*> allObjects;


    for (auto &pair: currentLevel->objects) {
        for (auto object: pair.second) {
            allObjects.push_back(object);
        }
    }


    for (int i = 0; i < allObjects.size(); i++) {
        for (int j = i; j < allObjects.size(); j++) {

            Object *object1 = allObjects[i];
            Object *object2 = allObjects[j];

            if (object1 != object2 && object1->canCollide && object2->canCollide && (object1->canMove || object2->canMove)) {
               object1->checkCollision(object2);

            }
        }
    }

}

void Game::draw() {

    Renderer::drawScene(currentLevel->light,currentLevel->objects, this->drawHitboxes);


}

void Game::addRandomCube() {

}

void Game::addRandomBunny() {

}

void Game::addRandomSphere() {

}

