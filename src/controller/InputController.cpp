//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_clip_space.hpp>
#include "InputController.h"

double InputController::lastX = 0;
double InputController::lastY = 0;
bool InputController::firstMouse = true;

InputController::InputController(GLFWwindow *window) {


    this->registerKeyCallback(window);
    this->registerMouseButtonCallback(window);
    this->registerCursorPosCallback(window);
    this->registerScrollCallback(window);
    this->registerReshapeCallback(window);

}

void InputController::registerKeyCallback(GLFWwindow *window) {
    glfwSetKeyCallback(window, InputController::keyCallback);
}

void InputController::registerMouseButtonCallback(GLFWwindow *window) {
    glfwSetMouseButtonCallback(window, InputController::mouseButtonCallback);
}

void InputController::registerCursorPosCallback(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, InputController::cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputController::registerScrollCallback(GLFWwindow *window) {
    glfwSetScrollCallback(window, InputController::scrollCallback);
}

void InputController::registerReshapeCallback(GLFWwindow *window) {
    glfwSetFramebufferSizeCallback(window, InputController::reshapeCallback);
}


void InputController::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        InputController::keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        InputController::keys[key] = false;
    }

    // Iterate over keys and do actions for true ones

    for (auto const &pressed_key: InputController::keys) {
        if (pressed_key.second) {
            if (pressed_key.first == GLFW_KEY_W) {
                Camera::getActiveInstance()->setMovingForward(true);
            } else if (pressed_key.first == GLFW_KEY_S) {
                Camera::getActiveInstance()->setMovingBackward(true);
            } else if (pressed_key.first == GLFW_KEY_A) {
                Camera::getActiveInstance()->setMovingLeft(true);
            } else if (pressed_key.first == GLFW_KEY_D) {
                Camera::getActiveInstance()->setMovingRight(true);
            } else if (pressed_key.first == GLFW_KEY_SPACE) {
                Camera::getActiveInstance()->setMovingUp(true);
            } else if (pressed_key.first == GLFW_KEY_LEFT_SHIFT) {
                Camera::getActiveInstance()->setMovingDown(true);
            } else if (pressed_key.first == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, true);
            } else if (pressed_key.first == GLFW_KEY_R) {
                Camera::getActiveInstance()->reset();
            } else if (pressed_key.first == GLFW_KEY_P) { ;;
                Game::pokeObjects();
                //publishToTopic(Topics::POKE_OBJECTS, "");
            } else if (pressed_key.first == GLFW_KEY_T) {
                Renderer::nextDrawMode();
            } else if (pressed_key.first == GLFW_KEY_Y) {
                Renderer::nextFocusedDrawMode();
            } else if (pressed_key.first == GLFW_KEY_C) {
                Game::addRandomCube();
            } else if (pressed_key.first == GLFW_KEY_B) {
                Game::addRandomBunny();
            } else if (pressed_key.first == GLFW_KEY_V) {
                Game::addRandomSphere();
            } else if (pressed_key.first == GLFW_KEY_I) {
                WindowController::getInstance().setWidth(WindowController::getInstance().getWidth() - 10);
            } else if (pressed_key.first == GLFW_KEY_O) {
                WindowController::getInstance().setWidth(WindowController::getInstance().getWidth() + 10);
            } else if (pressed_key.first == GLFW_KEY_LEFT_ALT) {
                if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
            }

        }
    }
}


void InputController::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        InputController::mouseButtons[button] = true;
        double x, y;
        glfwGetCursorPos(window, &x, &y);

    } else if (action == GLFW_RELEASE) {
        InputController::mouseButtons[button] = false;
    }

}

void InputController::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {

    if (InputController::firstMouse) {
        InputController::lastX = xpos;
        InputController::lastY = ypos;
        InputController::firstMouse = false;
    }

    double xoffset = xpos - InputController::lastX;
    double yoffset = InputController::lastY - ypos; // reversed since y-coordinates go from bottom to top

    InputController::lastX = xpos;
    InputController::lastY = ypos;


    Camera::getActiveInstance()->offsetOrientation(xoffset,yoffset);
}

void InputController::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    // Adjust the fov based on the yoffset of the scroll wheel
    Camera::getActiveInstance()->offsetFOV(yoffset);

}

void InputController::reshapeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

}

