//
// Created by ofaru on 16.03.2024.
//

#include "Game.h"

void Game::addObject(const Object &object) {



}

void Game::pokeObjects() {



}

void Game::update(GLfloat dt) {


    for (auto &pair : currentLevel.objects) {

        for (auto &object : pair.second) {

            object.update(dt);

        }

    }


}

void Game::setupLevels() {

}

void Game::checkCollisions() {

}

void Game::draw(Renderer &renderer) {

    for (auto &pair : currentLevel.objects) {

        for (auto &object : pair.second) {

            renderer.drawObject(object);

        }

    }

}

void Game::addRandomCube() {

}

void Game::addRandomBunny() {

}

void Game::addRandomSphere() {

}

