//
// Created by ofaru on 16.03.2024.
//

#include <memory>
#include "InputController.h"

double InputController::lastX = 0;
double InputController::lastY = 0;
bool InputController::firstMouse = true;

void InputController::init(GLFWwindow *window) {

    glfwSetKeyCallback(window, InputController::keyCallback);
    glfwSetMouseButtonCallback(window, InputController::mouseButtonCallback);
    glfwSetCursorPosCallback(window, InputController::cursorPosCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, InputController::scrollCallback);
    glfwSetFramebufferSizeCallback(window, InputController::reshapeCallback);


    auto *cameraKeyEventAdapter = new KeyEventAdapter(
            [](int key) {
                switch (key) {
                    case GLFW_KEY_W:
                        Camera::getActiveInstance()->setMovingForward(true);
                        break;
                    case GLFW_KEY_S:
                        Camera::getActiveInstance()->setMovingBackward(true);
                        break;
                    case GLFW_KEY_A:
                        Camera::getActiveInstance()->setMovingLeft(true);
                        break;
                    case GLFW_KEY_D:
                        Camera::getActiveInstance()->setMovingRight(true);
                        break;
                    case GLFW_KEY_SPACE:
                        Camera::getActiveInstance()->setMovingUp(true);
                        break;
                    case GLFW_KEY_LEFT_SHIFT:
                        Camera::getActiveInstance()->setMovingDown(true);
                        break;
                    case GLFW_KEY_R:
                        Camera::getActiveInstance()->reset();
                        break;
                    default:
                        break;
                }
            },
            [](int key) {
                switch (key) {
                    case GLFW_KEY_W:
                        Camera::getActiveInstance()->setMovingForward(false);
                        break;
                    case GLFW_KEY_S:
                        Camera::getActiveInstance()->setMovingBackward(false);
                        break;
                    case GLFW_KEY_A:
                        Camera::getActiveInstance()->setMovingLeft(false);
                        break;
                    case GLFW_KEY_D:
                        Camera::getActiveInstance()->setMovingRight(false);
                        break;
                    case GLFW_KEY_SPACE:
                        Camera::getActiveInstance()->setMovingUp(false);
                        break;
                    case GLFW_KEY_LEFT_SHIFT:
                        Camera::getActiveInstance()->setMovingDown(false);
                        break;
                    default:
                        break;
                }
            },
            [](int key) {
                switch (key) {
                    case GLFW_KEY_W:
                        Camera::getActiveInstance()->setMovingForward(true);
                        break;
                    case GLFW_KEY_S:
                        Camera::getActiveInstance()->setMovingBackward(true);
                        break;
                    case GLFW_KEY_A:
                        Camera::getActiveInstance()->setMovingLeft(true);
                        break;
                    case GLFW_KEY_D:
                        Camera::getActiveInstance()->setMovingRight(true);
                        break;
                    case GLFW_KEY_SPACE:
                        Camera::getActiveInstance()->setMovingUp(true);
                        break;
                    case GLFW_KEY_LEFT_SHIFT:
                        Camera::getActiveInstance()->setMovingDown(true);
                        break;
                    default:
                        break;
                }
            }
    );

    InputController::addKeyEventAdapter(cameraKeyEventAdapter);

    auto *cameraCursorMovedEventAdapter = new CursorMovedEventAdapter(
            [](double xpos, double ypos, double xoffset, double yoffset) {
                Camera::getActiveInstance()->offsetOrientation(xoffset, yoffset);
            }
    );

    InputController::addCursorMovedEventAdapter(cameraCursorMovedEventAdapter);

}


void InputController::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (auto adapter: InputController::keyEventAdapters) {
        if (action == GLFW_PRESS) {
            adapter->keyPressed(key);
        } else if (action == GLFW_RELEASE) {
            adapter->keyReleased(key);
        } else if (action == GLFW_REPEAT) {
            adapter->keyHeld(key);
        }
    }
}


void InputController::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    for (auto adapter: InputController::mouseButtonEventAdapters) {
        if (action == GLFW_PRESS) {
            adapter->mouseButtonPressed(button, x, y);
        } else if (action == GLFW_RELEASE) {
            adapter->mouseButtonReleased(button, x, y);
        }
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

    for (auto adapter: InputController::cursorMovedEventAdapters) {
        adapter->cursorMoved(xpos, ypos, xoffset, yoffset);
    }

}

void InputController::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    // Adjust the fov based on the yoffset of the scroll wheel
    Camera::getActiveInstance()->offsetFOV(-yoffset);
    Renderer::getActiveInstance()->createAndSetPerspectiveProjectionMatrix(WindowController::getInstance()->getWidth(),
                                                                           WindowController::getInstance()->getHeight());

}

void InputController::reshapeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer::getActiveInstance()->createAndSetPerspectiveProjectionMatrix(width, height);
}

void InputController::addKeyEventAdapter(KeyEventAdapter *keyEventAdapter) {
    keyEventAdapters.insert(keyEventAdapter);
}

void InputController::removeKeyEventAdapter(KeyEventAdapter *keyEventAdapter) {
    keyEventAdapters.erase(keyEventAdapter);
}

void InputController::addMouseButtonEventAdapter(MouseButtonEventAdapter *mouseButtonEventAdapter) {
    mouseButtonEventAdapters.insert(mouseButtonEventAdapter);
}

void InputController::removeMouseButtonEventAdapter(MouseButtonEventAdapter *mouseButtonEventAdapter) {
    mouseButtonEventAdapters.erase(mouseButtonEventAdapter);
}

void InputController::addCursorMovedEventAdapter(CursorMovedEventAdapter *cursorMovedEventAdapter) {
    cursorMovedEventAdapters.insert(cursorMovedEventAdapter);
}

void InputController::removeCursorMovedEventAdapter(CursorMovedEventAdapter *cursorMovedEventAdapter) {
    cursorMovedEventAdapters.erase(cursorMovedEventAdapter);
}

