//
// Created by ofaru on 12.03.2024.
//

#include "input.h"

std::map<int, bool> InputHandler::keys;
std::map<int, bool> InputHandler::mouseButtons;
Camera InputHandler::camera;
Renderer InputHandler::renderer;
double InputHandler::lastX = 0.0;
double InputHandler::lastY = 0.0;
bool InputHandler::firstMouse = true;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    std::cout << "key: " << key << " action: " << action << std::endl;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        InputHandler::keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        InputHandler::keys[key] = false;
    }

    // Iterate over keys and do actions for true ones

    for (auto const &pressed_key : InputHandler::keys) {
        if (pressed_key.second) {
            if (pressed_key.first == GLFW_KEY_W) {
                InputHandler::camera.moveForward(1.0f);
            } else if (pressed_key.first == GLFW_KEY_S) {
                InputHandler::camera.moveForward(-1.0f);
            } else if (pressed_key.first == GLFW_KEY_A) {
                InputHandler::camera.moveLeft(1.0f);
            } else if (pressed_key.first == GLFW_KEY_D) {
                InputHandler::camera.moveLeft(-1.0f);
            } else if (pressed_key.first == GLFW_KEY_SPACE) {
                InputHandler::camera.moveY(1.0f);
            } else if (pressed_key.first == GLFW_KEY_LEFT_SHIFT) {
                InputHandler::camera.moveY(-1.0f);
            } else if (pressed_key.first == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, true);
            } else if (pressed_key.first == GLFW_KEY_R) {
                InputHandler::camera.position = {0.0f, 0.0f, InputHandler::renderer.boundingBoxWidth/2};
                InputHandler::camera.yaw = -90.0f;
                InputHandler::camera.pitch = -0.0f;
                InputHandler::camera.fov = 90.0f;
                InputHandler::renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);
                InputHandler::renderer.createAndSetPerspectiveProjectionMatrix(InputHandler::renderer.windowWidth, InputHandler::renderer.windowHeight, InputHandler::camera.fov);
            }

        }
    }

    InputHandler::renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);

}


void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        InputHandler::mouseButtons[button] = true;
        double x, y;
        glfwGetCursorPos(window, &x, &y);

    } else if (action == GLFW_RELEASE) {
        InputHandler::mouseButtons[button] = false;
    }

}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {

    if (InputHandler::firstMouse) {
        InputHandler::lastX = xpos;
        InputHandler::lastY = ypos;
        InputHandler::firstMouse = false;
    }

    double xoffset = xpos - InputHandler::lastX;
    double yoffset = InputHandler::lastY - ypos; // reversed since y-coordinates go from bottom to top

    InputHandler::lastX = xpos;
    InputHandler::lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    InputHandler::camera.yaw += xoffset;
    InputHandler::camera.pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (InputHandler::camera.pitch > 89.0f)
        InputHandler::camera.pitch = 89.0f;
    if (InputHandler::camera.pitch < -89.0f)
        InputHandler::camera.pitch = -89.0f;

    InputHandler::renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Adjust the fov based on the yoffset of the scroll wheel
    InputHandler::camera.fov -= yoffset;

    // Make sure the fov stays within the range of 1 and 180
    if (InputHandler::camera.fov < 30.0f)
        InputHandler::camera.fov = 30.0f;
    if (InputHandler::camera.fov > 145.0f)
        InputHandler::camera.fov = 145.0f;

    std::cout << "fov: " << InputHandler::camera.fov << std::endl;
    std::cout << "window width: " << InputHandler::renderer.windowWidth << std::endl;
    std::cout << "window height: " << InputHandler::renderer.windowHeight << std::endl;

    InputHandler::renderer.createAndSetPerspectiveProjectionMatrix(InputHandler::renderer.windowWidth, InputHandler::renderer.windowHeight, InputHandler::camera.fov);

}

InputHandler::InputHandler(GLFWwindow *window, Game &game, Renderer &_renderer, Camera &_camera) {
    InputHandler::camera = _camera;
    std::cout << "Old renderer: " << InputHandler::renderer.windowWidth << std::endl;
    InputHandler::renderer = _renderer;
    std::cout << "New renderer: " << InputHandler::renderer.windowWidth << std::endl;
    registerKeyCallback(window);
    registerMouseButtonCallback(window);
    registerCursorPosCallback(window);
    registerScrollCallback(window);
}

void InputHandler::registerScrollCallback(GLFWwindow* window) {
    std::cout << "registering scroll callback" << std::endl;
    glfwSetScrollCallback(window, scrollCallback);
}

void InputHandler::registerKeyCallback(GLFWwindow *window) {
    std::cout << "registering key callback" << std::endl;
    glfwSetKeyCallback(window, keyCallback);
}

void InputHandler::registerMouseButtonCallback(GLFWwindow *window) {
    std::cout << "registering mouse button callback" << std::endl;
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void InputHandler::registerCursorPosCallback(GLFWwindow *window) {
    std::cout << "registering cursor position callback" << std::endl;
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool InputHandler::isKeyPressed(int key) {
    return keys[key];
}

bool InputHandler::isMouseButtonPressed(int button) {
    return mouseButtons[button];
}



