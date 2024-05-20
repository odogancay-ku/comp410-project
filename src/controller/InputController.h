//
// Created by ofaru on 16.03.2024.
//

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H


#include "camera/Camera.h"
#include <map>
#include <GLFW/glfw3.h>
#include <set>
#include "WindowController.h"
#include "camera/Camera.h"
#include "objects/physics/Object.h"
#include "game/Game.h"
#include "KeyEventAdapter.h"
#include "MouseButtonEventAdapter.h"
#include "CursorMovedEventAdapter.h"

class InputController {
public:
    static void init(GLFWwindow *window);

    static inline std::map<int, bool> mouseButtons = std::map<int, bool>();

    static inline std::set<KeyEventAdapter *> keyEventAdapters = std::set<KeyEventAdapter *>();
    static inline std::set<MouseButtonEventAdapter *> mouseButtonEventAdapters = std::set<MouseButtonEventAdapter *>();
    static inline std::set<CursorMovedEventAdapter *> cursorMovedEventAdapters = std::set<CursorMovedEventAdapter *>();

    static double lastX;
    static double lastY;
    static bool firstMouse;

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    static void reshapeCallback(GLFWwindow *window, int width, int height);

    static void addKeyEventAdapter(KeyEventAdapter *keyEventAdapter);

    static void removeKeyEventAdapter(KeyEventAdapter *keyEventAdapter);

    static void addMouseButtonEventAdapter(MouseButtonEventAdapter *mouseButtonEventAdapter);

    static void removeMouseButtonEventAdapter(MouseButtonEventAdapter *mouseButtonEventAdapter);

    static void addCursorMovedEventAdapter(CursorMovedEventAdapter *cursorMovedEventAdapter);

    static void removeCursorMovedEventAdapter(CursorMovedEventAdapter *cursorMovedEventAdapter);

};

#endif //INPUTCONTROLLER_H
