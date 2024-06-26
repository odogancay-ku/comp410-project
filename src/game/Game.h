
#ifndef GAME_H
#define GAME_H


#include <vector>
#include <memory>
#include "objects/physics/Object.h"
#include "renderer/Renderer.h"
#include "persistent/level/Level.h"

class Game {

private:

    Game() {
        currentLevel = nullptr;
        levels = std::vector<std::shared_ptr<Level>>();
        setupLevels();
    };

    static Game* instance;




public:

    bool drawHitboxes = false;

    std::vector<std::shared_ptr<Level>> levels;

    std::shared_ptr<Level> currentLevel;


    static Game* getInstance() {
        if (instance == nullptr) {
            instance = new Game();
        }
        return instance;
    }

    void setupLevels();

    void update(GLfloat dt);

    void checkCollisions();

    void draw();



    static void pokeObjects();

    static void addRandomCube();

    static void addRandomBunny();

    static void addRandomSphere();

};


#endif //GAME_H
