//
// Created by ofaru on 12.03.2024.
//

#include "input.h"
#include "Angel.h"

std::map<int, bool> InputHandler::keys;
std::map<int, bool> InputHandler::mouseButtons;
Camera InputHandler::camera;
Renderer InputHandler::renderer;
Game* InputHandler::game;
std::vector<Object>* InputHandler::objects;
int InputHandler::drawMode = 0;
double InputHandler::lastX = 0.0;
double InputHandler::lastY = 0.0;
bool InputHandler::firstMouse = true;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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
                InputHandler::camera.position = {0.0f, 0.0f, InputHandler::renderer.boundingBoxWidth};
                InputHandler::camera.yaw = -90.0f;
                InputHandler::camera.pitch = -0.0f;
                InputHandler::camera.fov = 90.0f;
                InputHandler::renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);
                InputHandler::renderer.createAndSetPerspectiveProjectionMatrix(InputHandler::renderer.windowWidth, InputHandler::renderer.windowHeight, InputHandler::camera.fov);
            } else if (pressed_key.first == GLFW_KEY_P) {
                // Give a little velocity to non static objects
                for (auto &object: *InputHandler::objects) {
                    if (!object.isStatic) {
                        // Give random velocity to the object
                        float maxVelocity = 25.0f;
                        object.velocity.x = (rand() % 100) / 100.0f * maxVelocity*2- maxVelocity;
                        object.velocity.y = (rand() % 100) / 100.0f * maxVelocity*2- maxVelocity;
                        object.velocity.z = (rand() % 100) / 100.0f * maxVelocity*2- maxVelocity;
                    }
                }
            } else if (pressed_key.first == GLFW_KEY_T) {

                InputHandler::drawMode++;

                switch (InputHandler::drawMode) {

                    case 0:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        break;
                    case 1:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        break;
                    case 2:
                        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                        break;
                    default:
                        InputHandler::drawMode = 0;
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        break;

                }

            } else if (pressed_key.first == GLFW_KEY_C) {

                // Create a new cube at origin with random speed and color
                vec3 speed = {0, 0, 0};
                speed.x = (rand() % 100) / 100.0f * 5.0f;
                speed.y = (rand() % 100) / 100.0f * 5.0f;
                speed.z = (rand() % 100) / 100.0f * 5.0f;

                vec3 color = {0, 0, 0};
                color.x = (rand() % 100) / 100.0f;
                color.y = (rand() % 100) / 100.0f;
                color.z = (rand() % 100) / 100.0f;

                float size = (rand() % 100) / 100.0f * 2.0f+0.2f;

                Cube cube({0, 0, 0}, color, size);
                cube.velocity = speed;
                cube.restitution = 0.8f;
                InputHandler::game->addObject(cube);

            } else if (pressed_key.first == GLFW_KEY_B) {
                // Create a new bunny at origin with random speed and color
                vec3 speed = {0, 0, 0};
                speed.x = (rand() % 100) / 100.0f * 5.0f;
                speed.y = (rand() % 100) / 100.0f * 5.0f;
                speed.z = (rand() % 100) / 100.0f * 5.0f;

                vec3 color = {0, 0, 0};
                color.x = (rand() % 100) / 100.0f;
                color.y = (rand() % 100) / 100.0f;
                color.z = (rand() % 100) / 100.0f;

                float size = (rand() % 100) / 100.0f * 2.0f+0.2f;

                Bunny bunny({0, 0, 0}, color, size);
                bunny.velocity = speed;
                bunny.restitution = 0.8f;
                InputHandler::game->addObject(bunny);
            } else if (pressed_key.first == GLFW_KEY_V) {
                // Create a new sphere at origin with random speed and color
                vec3 speed = {0, 0, 0};
                speed.x = (rand() % 100) / 100.0f * 5.0f;
                speed.y = (rand() % 100) / 100.0f * 5.0f;
                speed.z = (rand() % 100) / 100.0f * 5.0f;

                vec3 color = {0, 0, 0};
                color.x = (rand() % 100) / 100.0f;
                color.y = (rand() % 100) / 100.0f;
                color.z = (rand() % 100) / 100.0f;

                float size = (rand() % 100) / 100.0f * 1.0f+0.2f;

                Sphere sphere({0, 0, 0}, color, size);
                sphere.velocity = speed;
                sphere.restitution = 0.8f;
                InputHandler::game->addObject(sphere);
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

    InputHandler::renderer.createAndSetPerspectiveProjectionMatrix(InputHandler::renderer.windowWidth, InputHandler::renderer.windowHeight, InputHandler::camera.fov);

}

InputHandler::InputHandler(GLFWwindow *window, Game &_game, Renderer &_renderer, Camera &_camera) {
    InputHandler::camera = _camera;
    InputHandler::renderer = _renderer;
    InputHandler::game = &_game;
    InputHandler::objects = &_game.objects;
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



