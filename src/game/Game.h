//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_GAME_H
#define HOMEWORK1_GAME_H


#include <vector>
#include "../objects/physics/Object.h"
#include "../renderer/Renderer.h"
#include "level/Level.h"

class Game {

private:

    std::vector<Level*> levels;

    Level* currentLevel;

    Game() {
        std::cout << "Game created" << std::endl;
        currentLevel = nullptr;
        levels = std::vector<Level*>();
        setupLevels();
    };





public:

    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    void setupLevels();

    void addObject(const Object &object);

    void update(GLfloat dt);

    void checkCollisions();

    void draw();



    static void pokeObjects();

    static void addRandomCube();

    static void addRandomBunny();

    static void addRandomSphere();
};


#endif //HOMEWORK1_GAME_H
