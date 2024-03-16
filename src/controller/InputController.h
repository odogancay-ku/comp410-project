//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_INPUTCONTROLLER_H
#define HOMEWORK1_INPUTCONTROLLER_H


#include "../camera/Camera.h"
#include <map>
#include <GLFW/glfw3.h>
#include "WindowController.h"
#include "../camera/Camera.h"
#include "../objects/physics/Object.h"
#include "../objects/physics/Sphere.h"
#include "../game/Game.h"

class InputController {
public:
    InputController(GLFWwindow *window);

    static std::map<int, bool> keys;
    static std::map<int, bool> mouseButtons;

    void registerKeyCallback(GLFWwindow *window);

    void registerMouseButtonCallback(GLFWwindow *window);

    void registerCursorPosCallback(GLFWwindow *window);

    void registerScrollCallback(GLFWwindow *window);

    void registerReshapeCallback(GLFWwindow *window);

    static double lastX;
    static double lastY;
    static bool firstMouse;
    static Camera camera;
    static int drawMode;

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    void reshapeCallback(GLFWwindow *window, int width, int height);

};

#endif //HOMEWORK1_INPUTCONTROLLER_H
