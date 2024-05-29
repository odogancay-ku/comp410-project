
#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H


#include <vector>
#include <GLFW/glfw3.h>

class WindowController {


public:

    static inline int width = 0;
    static inline int height = 0;

    static inline GLFWwindow *window = nullptr;

    static void createFullscreenWindow(const char *title) {

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
        window = glfwCreateWindow(windowWidth, windowHeight, title, nullptr, nullptr);
        if (!window) {
            // Window or OpenGL context creation failed
            glfwTerminate();
            std::cerr << "Window or OpenGL context creation failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);


    }

};


#endif //WINDOWCONTROLLER_H
