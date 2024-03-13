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
        if (key == GLFW_KEY_W) {
            InputHandler::camera.moveX(1.0f);
        } else if (key == GLFW_KEY_S) {
            InputHandler::camera.moveX(-1.0f);
        } else if (key == GLFW_KEY_A) {
            InputHandler::camera.moveZ(-1.0f);
        } else if (key == GLFW_KEY_D) {
            InputHandler::camera.moveZ(1.0f);
        } else if (key == GLFW_KEY_SPACE) {
            InputHandler::camera.moveY(1.0f);
        } else if (key == GLFW_KEY_LEFT_SHIFT) {
            InputHandler::camera.moveY(-1.0f);
        }
    } else if (action == GLFW_RELEASE) {
        InputHandler::keys[key] = false;
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

    std::cout << "Camera position: " << InputHandler::camera.position.x << " " << InputHandler::camera.position.y << " "
              << InputHandler::camera.position.z << std::endl;
    std::cout << "Camera yaw:" << InputHandler::camera.yaw << " pitch: " << InputHandler::camera.pitch << std::endl;

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

    std::cout << "Camera position: " << InputHandler::camera.position.x << " " << InputHandler::camera.position.y << " "
              << InputHandler::camera.position.z << std::endl;
    std::cout << "Camera yaw:" << InputHandler::camera.yaw << " pitch: " << InputHandler::camera.pitch << std::endl;


    InputHandler::renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);

}


InputHandler::InputHandler(GLFWwindow *window, Game &game, Renderer &_renderer, Camera &_camera) {
    InputHandler::camera = _camera;
    InputHandler::renderer = _renderer;
    registerKeyCallback(window);
    registerMouseButtonCallback(window);
    registerCursorPosCallback(window);
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



