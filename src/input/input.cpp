//
// Created by ofaru on 12.03.2024.
//

#include "input.h"

std::map<int, bool> InputHandler::keys;
std::map<int, bool> InputHandler::mouseButtons;

InputHandler::InputHandler(GLFWwindow* window, Game &game, Renderer &renderer) {
    registerKeyCallback(window);
    registerMouseButtonCallback(window);
}

void InputHandler::registerKeyCallback(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
}

void InputHandler::registerMouseButtonCallback(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

bool InputHandler::isKeyPressed(int key) {
    return keys[key];
}

bool InputHandler::isMouseButtonPressed(int button) {
    return mouseButtons[button];
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS)
        mouseButtons[button] = true;
    else if (action == GLFW_RELEASE)
        mouseButtons[button] = false;
}
