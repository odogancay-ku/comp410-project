//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef COMP410_PROJECT_HW3_H
#define COMP410_PROJECT_HW3_H

#include <iostream>
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"
#include "renderer/Renderer.h"
#include "controller/InputController.h"

struct Light {
    glm::vec3 lightPos;
    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;

} typedef;

class HW3 : public Level {


public:

    Light* light;

    Object *dumbObject;

    void nextDumbObject() {
        // Use the next enum, if it's the last one, use the first one
        dumbObject->modelType = (ModelTypes) ((dumbObject->modelType + 1) %
                                              ((int) ModelTypes::UNIQUE_MODEL));
    }

    HW3() {

    }

    static inline std::vector<GLuint> shaderPrograms = {};

    static inline std::vector<char *> vertexShaderPaths = {
            "shaders/blinnPhongVertex.glsl",
            "shaders/gouraudVertex.glsl",
            "shaders/phongVertex.glsl"
    };

    static inline std::vector<char *> fragmentShaderPaths = {
            "shaders/blinnPhongFragment.glsl",
            "shaders/gouraudFragment.glsl",
            "shaders/phongFragment.glsl"
    };

    static inline int shaderIndex = 0;

    void setup() override {

        std::cout << "Creating persistent HW3" << std::endl;


        light = new Light;

        float roomSize = 15;


        glm::vec3 lightPos = glm::vec3(roomSize, roomSize / 4, 0.0f);
//        glm::vec3 lightPos = glm::vec3(0.0f, roomSize/4   , 0.0f);

        glm::vec3 lightAmbient = {0.1f, 0.1f, 0.1f};
        glm::vec3 lightDiffuse = {1.0f, 1.0f, 1.0f};
        glm::vec3 lightSpecular = {0.2f, 0.2f, 0.2f};

        light->lightPos=lightPos;
        light->lightAmbient=lightAmbient;
        light->lightDiffuse=lightDiffuse;
        light->lightSpecular=lightSpecular;

        Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);



        std::cout << "Level created" << std::endl;

        std::cout << "Creating environment" << std::endl;

        environment = new Earth();

        std::cout << "Environment created" << std::endl;

        std::cout << "Adding objects" << std::endl;


        auto *cube = new Object();
        cube->modelType = ModelTypes::UNIQUE_MODEL;
        cube->modelData = generateCubeModelData(glm::vec3(1.0f, 1.0f, 1.0f));
        cube->position = glm::vec3(roomSize, -roomSize / 2, 0.0f);
        cube->canMove = false;
        cube->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        cube->paint(0.2f, 0.2f, 0.4f);
        addObject(cube);

        auto *cube2 = new Object();
        cube2->modelType = ModelTypes::UNIQUE_MODEL;
        cube2->modelData = generateCubeModelData(glm::vec3(1.0f, 0.0f, 1.0f));
        cube2->position = glm::vec3(roomSize, +roomSize / 2, 0.0f);
        cube2->canMove = false;
        cube2->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        addObject(cube2);

        auto *cube3 = new Object();
        cube3->modelType = ModelTypes::UNIQUE_MODEL;
        cube3->modelData = generateCubeModelData(glm::vec3(1.0f, 0.0f, 1.0f));
        cube3->position = glm::vec3(roomSize, 0.0f, -roomSize / 2 - 0.5);
        cube3->canMove = false;
        cube3->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        cube3->paint(0.4f, 0.2f, 0.2f);
        addObject(cube3);

        auto *cube4 = new Object();
        cube4->modelType = ModelTypes::UNIQUE_MODEL;
        cube4->modelData = generateCubeModelData(glm::vec3(1.0f, 1.0f, 0.0f));
        cube4->position = glm::vec3(roomSize, 0.0f, +roomSize / 2 + 0.5);
        cube4->canMove = false;
        cube4->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        cube4->paint(0.4f, 0.2f, 0.2f);
        addObject(cube4);

        auto *cube5 = new Object();
        cube5->modelType = ModelTypes::UNIQUE_MODEL;
        cube5->modelData = generateCubeModelData(glm::vec3(1.0f, 1.0f, 0.0f));
        cube5->position = glm::vec3(roomSize * 3 / 2 + 0.5, 0.0f, 0.0f);
        cube5->canMove = false;
        cube5->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube5->paint(0.4f, 0.2f, 0.2f);
        addObject(cube5);

        auto *cube6 = new Object();
        cube6->modelType = ModelTypes::UNIQUE_MODEL;
        cube6->modelData = generateCubeModelData(glm::vec3(0.0f, 1.0f, 1.0f));
        cube6->position = glm::vec3(roomSize / 2 - 0.5, 0.0f, 0.0f);
        cube6->canMove = false;
        cube6->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube6->isHidden = true;
        addObject(cube6);

        dumbObject = new Object();
        dumbObject->modelType = ModelTypes::SPHERE;
        dumbObject->position = glm::vec3(roomSize, -roomSize / 2 + 0.5f, -4.0f);
        dumbObject->velocity = glm::vec3(3.0f, 2.0f, 4.0f);
        dumbObject->canMove = true;
        dumbObject->scale = 3.0;
        dumbObject->mass = 1.0;
        dumbObject->applyGravity = false;
        dumbObject->restitution = 1.0;
        dumbObject->paint(1.0f, 0.0f, 0.0f);
        addObject(dumbObject);


        std::cout << "WELCOME! Press P to give a little velocity to non static objects" << std::endl;
        std::cout << "Press L to change the object model" << std::endl;
        std::cout << "Press H to show hitboxes, you might need to switch to point or line draw mode to see them"
                  << std::endl;
        std::cout << "Press T to change the draw mode" << std::endl;
        std::cout << "Press ESC to exit" << std::endl;
        std::cout << "Press W, A, S, D, left shift, space to move the camera relative to the orientation" << std::endl;
        std::cout << "Press R to reset the camera" << std::endl;
        std::cout << "You can change the way you face using your mouse! Just drag it, no roll only yaw and pitch"
                  << std::endl;
        std::cout
                << "For this to work we had to hide the mouse and fix it. "
                   "Press alt to toggle this. "
                   "You will need to test out the reshape callback functionality. "
                   "Press alt, go out of full screen, "
                   "resize with the mouse "
                   "This will be enough to showcase window resize callback"
                << std::endl;
        std::cout << "Use the mouse wheel to change the fov" << std::endl;
        std::cout << "Have fun!" << std::endl;

        for (int i = 0; i < vertexShaderPaths.size(); i++) {
            shaderPrograms.push_back(
                    Renderer::getActiveInstance()->loadShaderProgram(vertexShaderPaths[i], fragmentShaderPaths[i]));
        }


        InputController::addKeyEventAdapter(new KeyEventAdapter(

                [this](int key) {
                    switch (key) {
                        case GLFW_KEY_C:
                            shaderIndex = (shaderIndex + 1) % vertexShaderPaths.size();
                            std::cout << "Loading shader program: " << shaderPrograms[shaderIndex] <<  std::endl;
                            Renderer::getActiveInstance()->useShaderProgram(shaderPrograms[shaderIndex]);
                            Renderer::getActiveInstance()->setLight(light->lightPos,light->lightAmbient,light->lightDiffuse,light->lightDiffuse);

                            break;
                        case GLFW_KEY_O:
                            nextDumbObject();
                            break;
                        default:
                            break;
                    }
                },
                [](int key) {
                },
                [](int key) {
                }

        ));

    }

    void onUpdate(float dt) override {

    }

};


#endif //COMP410_PROJECT_HW3_H
