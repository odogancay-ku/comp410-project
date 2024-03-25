//
// Created by ofaru on 17.03.2024.
//

#ifndef HOMEWORK1_HW1_H
#define HOMEWORK1_HW1_H

#include <iostream>
#include "../../../objects/physics/Object.h"
#include "../../../objects/physics/Object.h"
#include "../environment/Earth.h"
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

    void setup() {

        float roomSize = 15;

        glm::vec3 lightPos = glm::vec3(roomSize, roomSize/4   , 0.0f);
//        glm::vec3 lightPos = glm::vec3(0.0f, roomSize/4   , 0.0f);

        GLint lightLoc = glGetUniformLocation(Renderer::getActiveInstance()->shaderProgram, "lightPos");
        glUniform3fv(lightLoc, 1,  glm::value_ptr(lightPos));


        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

        GLint lightColorLoc = glGetUniformLocation(Renderer::getActiveInstance()->shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1,  glm::value_ptr(lightColor));

        std::cout << "Creating persistent HW1" << std::endl;


        std::cout << "Level created" << std::endl;

        std::cout << "Creating environment" << std::endl;

        environment = new Earth();

        std::cout << "Environment created" << std::endl;

        std::cout << "Adding objects" << std::endl;




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
        dumbObject->rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
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


    }

};

#endif //HOMEWORK1_HW1_H
