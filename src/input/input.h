//
// Created by ofaru on 12.03.2024.
//

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <map>
#include "../game/game.h"
#include "../visuals/rendering.h"

class InputHandler {
public:
    InputHandler(GLFWwindow* window, Game &game, Renderer &renderer);
    void registerKeyCallback();
    void registerMouseButtonCallback();
    bool isKeyPressed(int key);
    bool isMouseButtonPressed(int button);

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static std::map<int, bool> keys;
    static std::map<int, bool> mouseButtons;

    void registerKeyCallback(GLFWwindow *window);

    void registerMouseButtonCallback(GLFWwindow *window);
};

#endif //INPUT_H
