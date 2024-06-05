
#ifndef GAME_H
#define GAME_H


#include <vector>
#include <memory>
#include "renderer/Renderer.h"

class Game {

private:

    Game() {
    };

    static Game* instance;




public:

    static Game* getInstance() {
        if (instance == nullptr) {
            instance = new Game();
        }
        return instance;
    }

    void setupLevels();

    void update(GLfloat dt);

    void checkCollisions();


};


#endif //GAME_H
