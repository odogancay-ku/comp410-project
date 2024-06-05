
#include <algorithm>
#include "Game.h"
#include "camera/Camera.h"

Game *Game::instance = nullptr;


void Game::update(GLfloat dt) {


    Camera *camera = Camera::getActiveInstance();

    camera->update(dt);

    glm::vec3 position = camera->position;

}

void Game::setupLevels() {
}

void Game::checkCollisions() {
}


