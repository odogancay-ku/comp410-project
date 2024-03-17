//
// Created by ofaru on 16.03.2024.
//

#include "Level.h"
#include "../../objects/graphics/ResourceManager.h"
#include "../../objects/physics/Sphere.h"
#include "../../objects/physics/Cube.h"
#include "../../camera/Camera.h"
#include "../../objects/physics/External.h"


Level *Level::HW1() {

    std::cout << "Creating level HW1" << std::endl;

    auto *level = new Level();

    std::cout << "Level created" << std::endl;

    std::cout << "Creating environment" << std::endl;

    level->environment = Environment::Earth();

    std::cout << "Environment created" << std::endl;

    std::cout << "Adding objects" << std::endl;




    auto *maid = new External(ModelTypes::MAID, 50.0f);
    maid->position = glm::vec3(0.0f, 0.0f, 100.0f);
    maid->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    maid->applyGravity = false;
    maid->canMove = false;
    maid->applyDrag = false;
    maid->angularVelocity = glm::vec3(0.0f, 100.0, 0.0f);
    level->addObject(*maid);



    float roomSize = 12;


    auto skyDome = new Sphere(200);
    skyDome->position = glm::vec3(0.0f, 0.0f, 0.0f);
    skyDome->canMove = false;
    level->addObject(*skyDome);

    auto *cube = new Cube(1.0f);
    cube->position = glm::vec3(roomSize, -roomSize/2, 0.0f);
    cube->canMove = false;
    cube->stretch= glm::vec3(roomSize, 1.0f, roomSize);
    level->addObject(*cube);

    auto *cube2 = new Cube(1.0f);
    cube2->position = glm::vec3(roomSize, +roomSize/2, 0.0f);
    cube2->canMove = false;
    cube2->stretch= glm::vec3(roomSize, 1.0f, roomSize);
    level->addObject(*cube2);

    auto *cube3 = new Cube(1.0f);
    cube3->position = glm::vec3(roomSize, 0.0f, -roomSize/2 -0.5);
    cube3->canMove = false;
    cube3->stretch= glm::vec3(roomSize, roomSize+1, 1.0f);
    level->addObject(*cube3);

    auto *cube4 = new Cube(1.0f);
    cube4->position = glm::vec3(roomSize, 0.0f, +roomSize/2 +0.5);
    cube4->canMove = false;
    cube4->stretch= glm::vec3(roomSize, roomSize+1, 1.0f);
    level->addObject(*cube4);

    auto *cube5 = new Cube(1.0f);
    cube5->position = glm::vec3(roomSize*3/2+0.5, 0.0f, 0.0f);
    cube5->canMove = false;
    cube5->stretch= glm::vec3(1.0f, roomSize+1, roomSize+2);
    level->addObject(*cube5);

    auto *cube6 = new Cube(1.0f);
    cube6->position = glm::vec3(roomSize/2-0.5, 0.0f, 0.0f);
    cube6->canMove = false;
    cube6->stretch= glm::vec3(1.0f, roomSize+1, roomSize+2);
    cube6->isHidden = true;
    level->addObject(*cube6);


    auto *dumbObject = new Object();
    dumbObject->modelType = ModelTypes::CUBE;
    dumbObject->position = glm::vec3(0.0f, 0.0f, roomSize);
    dumbObject->velocity = glm::vec3(1.0f, 1.0f, 1.0f);
    dumbObject->canMove = false;
    dumbObject->angularVelocity = glm::vec3(120.0f, 120.0f, 120.0f);
    dumbObject->scale = 1.0;
    level->addObject(*dumbObject);




//    Camera::getActiveInstance()->setFollowObject(maid);

//    Camera::getActiveInstance()->position = glm::vec3(-10.0f, 20.0f, 0.0f);


    std::cout << "Objects added" << std::endl;

    return level;
}

void Level::addObject(const Object &object) {

    objects[object.modelType].push_back(object);

}
