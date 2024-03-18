//
// Created by ofaru on 16.03.2024.
//

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include "WindowController.h"

WindowController* WindowController::instance = nullptr;

void WindowController::createFullscreenWindow(const char *title) {

    // Initialize GLFW
    if (!glfwInit()) {
        // Initialization failed
        std::cerr << "GLFW initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create a fullscreen window
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int windowWidth = mode->width;
    int windowHeight = mode->height;

    width = windowWidth;
    height = windowHeight;

    // Create a window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
    if (!window) {
        // Window or OpenGL context creation failed
        glfwTerminate();
        std::cerr << "Window or OpenGL context creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    windows.push_back(window);
    activeWindow = window;

}

void WindowController::createWindow(const char *title, int width, int height) {
    // Initialize GLFW
    if (!glfwInit()) {
        // Initialization failed
        std::cerr << "GLFW initialization failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->width = width;
    this->height = height;

    // Create a window
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        // Window or OpenGL context creation failed
        std::cerr << "Window or OpenGL context creation failed" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    windows.push_back(window);
    activeWindow = window;

}

GLFWwindow *WindowController::getActiveWindow() const {
    return activeWindow;
}

const std::vector<GLFWwindow *> &WindowController::getWindows() const {
    return windows;
}

int WindowController::getWidth() const {
    return width;
}

int WindowController::getHeight() const {
    return height;
}

void WindowController::setActiveWindow(GLFWwindow *activeWindow) {
    WindowController::activeWindow = activeWindow;
}

void WindowController::setWidth(int width) {
    WindowController::width = width;
}

void WindowController::setHeight(int height) {
    WindowController::height = height;
}
