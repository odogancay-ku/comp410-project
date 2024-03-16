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

    std::cout << "Camera updated at:" << position.x << "," << position.y << "," << position.z << std::endl;

    for (auto &pair : currentLevel->objects) {

        std::cout << "Updating objects with model " << pair.first << " of level" << std::endl;

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

    glm::mat4 viewMatrix;
    Camera::getActiveInstance()->getViewMatrix(viewMatrix);
    renderer->setViewMatrix(viewMatrix);

    for (auto &pair : currentLevel->objects) {

        std::cout << "Drawing objects with model: " << pair.first << std::endl;

        renderer->setModelMatrix(viewMatrix);

        renderer->drawInstancesOfModel(pair.first, &pair.second);


    }

}

void Game::addRandomCube() {

}

void Game::addRandomBunny() {

}

void Game::addRandomSphere() {

}

