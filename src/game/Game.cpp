//
// Created by ofaru on 16.03.2024.
//

#include "Game.h"
#include "../camera/Camera.h"

void Game::addObject(const Object &object) {



}

void Game::pokeObjects() {



}

void Game::update(GLfloat dt) {


    Camera* camera = Camera::getActiveInstance();

    camera->update(dt);

    glm::vec3 position = camera->position;

    for (auto &pair : currentLevel->objects) {

        for (auto &object : pair.second) {
            object.update(dt);
        }

    }


}

void Game::setupLevels() {
        std::cout << "Setting up levels" << std::endl;
        Level* level = Level::HW1();
        std::cout << "Level HW1 created" << std::endl;

        currentLevel = level;

        std::cout << "Levels created" << std::endl;

        levels.push_back(level);

        std::cout << "Levels setup finished" << std::endl;
}

void Game::checkCollisions() {

}

void Game::draw() {

    Renderer* renderer = Renderer::getActiveInstance();

    // Setup common uniforms

    renderer->createAndSetViewMatrix();


    // Check if perspective and view matrix uniforms are set



    for (auto &pair : currentLevel->objects) {

        renderer->drawInstancesOfModel(pair.first, &pair.second);

    }

}

void Game::addRandomCube() {

}

void Game::addRandomBunny() {

}

void Game::addRandomSphere() {

}

