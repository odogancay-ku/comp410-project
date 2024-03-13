//
// Created by ofaru on 12.03.2024.
//

#ifndef INPUT_H
#define INPUT_H

#include <map>
#include "../game/game.h"
#include "../visuals/rendering.h"
#include "../visuals/camera.h"

class InputHandler {
public:
    InputHandler(GLFWwindow* window, Game &game, Renderer &renderer, Camera &camera);
    bool isKeyPressed(int key);
    bool isMouseButtonPressed(int button);

    static std::map<int, bool> keys;
    static std::map<int, bool> mouseButtons;



    static void registerKeyCallback(GLFWwindow *window);

    static void registerMouseButtonCallback(GLFWwindow *window);

    static void registerCursorPosCallback(GLFWwindow *window);

    static void registerScrollCallback(GLFWwindow *window);

    static double lastX;
    static double lastY;
    static bool firstMouse;
    static Renderer renderer;
    static Camera camera;

};

#endif //INPUT_H
