//
// Created by ofaru on 17.03.2024.
//

#ifndef HW1_H
#define HW1_H

#include <iostream>
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"

class HW1 : public Level {


public:

    Object *dumbObject;

    void nextDumbObject() {
        // Use the next enum, if it's the last one, use the first one
        dumbObject->modelType = (ModelTypes) ((dumbObject->modelType + 1) %
                                              ((int) ModelTypes::END_OF_TYPES_MARKER));
    }

    HW1() {

    }

    void setup() override {

        float roomSize = 15;

        glm::vec3 lightPos = glm::vec3(roomSize, roomSize/4   , 0.0f);
//        glm::vec3 lightPos = glm::vec3(0.0f, roomSize/4   , 0.0f);

        glm::vec3 lightAmbient = {0.2f, 0.2f, 0.2f};
        glm::vec3 lightDiffuse = {0.5f, 0.5f, 0.5f};
        glm::vec3 lightSpecular = {1.0f, 1.0f, 1.0f};

        Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);


        environment = new Earth();



        auto *cube = new Object();
        cube->position = glm::vec3(roomSize, -roomSize / 2, 0.0f);
        cube->canMove = false;
        cube->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        cube->paint(0.2f,0.2f,0.4f);
        addObject(cube);

        auto *cube2 = new Object();
        cube2->position = glm::vec3(roomSize, +roomSize / 2, 0.0f);
        cube2->canMove = false;
        cube2->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        addObject(cube2);

        auto *cube3 = new Object();
        cube3->position = glm::vec3(roomSize, 0.0f, -roomSize / 2 - 0.5);
        cube3->canMove = false;
        cube3->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        cube3->paint(0.4f, 0.2f, 0.2f);
        addObject(cube3);

        auto *cube4 = new Object();
        cube4->position = glm::vec3(roomSize, 0.0f, +roomSize / 2 + 0.5);
        cube4->canMove = false;
        cube4->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        cube4->paint(0.4f, 0.2f, 0.2f);
        addObject(cube4);

        auto *cube5 = new Object();
        cube5->position = glm::vec3(roomSize * 3 / 2 + 0.5, 0.0f, 0.0f);
        cube5->canMove = false;
        cube5->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube5->paint(0.4f, 0.2f, 0.2f);
        addObject(cube5);

        auto *cube6 = new Object();
        cube6->position = glm::vec3(roomSize / 2 - 0.5, 0.0f, 0.0f);
        cube6->canMove = false;
        cube6->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube6->isHidden = true;
        addObject(cube6);

        dumbObject = new Object();
        dumbObject->modelType = ModelTypes::CUBE;
        dumbObject->position = glm::vec3(roomSize, -roomSize/2+0.5f, -4.0f);
        dumbObject->velocity = glm::vec3(0.0f, 0.0f, 4.0f);
//        dumbObject->angularVelocity = glm::vec3(50.0f, 0.0f, 0.0f);
        dumbObject->canMove = true;
        dumbObject->scale = 1.0;
        dumbObject->mass = 1.0;
        dumbObject->restitution = 1.0;
//        dumbObject->angularVelocity = glm::vec3 (0.0f, 0.0f, 50.0f);
        dumbObject->paint(1.0f,0.0f,0.0f);
        addObject(dumbObject);

        auto *cube7 = new Object();
        cube7->modelType = ModelTypes::CUBE;
        cube7->position = glm::vec3(roomSize, -roomSize/2+2.0f, 4.0f);
        cube7->canMove = true;
        cube7->scale = 4.0;
        cube7->mass = 64.0;
        cube7->restitution = 1.0;
        cube7->velocity = glm::vec3(0.0f, 0.0f, -4.0f);
        cube7->paint(0.0f,1.0f,0.0f);
        addObject(cube7);

        auto * maid = new Object();
        maid->modelType = ModelTypes::MAID;
        maid->paint(1.0f, 1.0f, 1.0f);
        maid->position = glm::vec3(roomSize, 0.0f, 0.0f);
        maid->angularVelocity = glm::vec3(0.0f, 50.0f, 0.0f);
        maid->canMove = false;
        maid->canCollide = false;
        maid->scale = 3.0f;
        addObject(maid);

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
    }

    void onUpdate(float dt) override {

    }

};

#endif //HW1_H
