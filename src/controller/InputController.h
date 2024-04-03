//
// Created by ofaru on 16.03.2024.
//

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H


#include "camera/Camera.h"
#include <map>
#include <GLFW/glfw3.h>
#include "WindowController.h"
#include "camera/Camera.h"
#include "objects/physics/Object.h"
#include "game/Game.h"

class InputController {
public:
    explicit InputController(GLFWwindow *window);

    static inline std::map<int, bool> keys = std::map<int, bool>();
    static inline std::map<int, bool> mouseButtons = std::map<int, bool>();

    void registerKeyCallback(GLFWwindow *window);

    void registerMouseButtonCallback(GLFWwindow *window);

    void registerCursorPosCallback(GLFWwindow *window);

    void registerScrollCallback(GLFWwindow *window);

    void registerReshapeCallback(GLFWwindow *window);

    static double lastX;
    static double lastY;
    static bool firstMouse;

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    static void reshapeCallback(GLFWwindow *window, int width, int height);

};

#endif //INPUTCONTROLLER_H
