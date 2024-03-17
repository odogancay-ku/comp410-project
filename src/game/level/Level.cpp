//
// Created by ofaru on 16.03.2024.
//

#include "Level.h"
#include "../../objects/physics/Sphere.h"
#include "../../objects/physics/Cube.h"
#include "../../camera/Camera.h"


Level* Level::HW1() {

    std::cout << "Creating level HW1" << std::endl;

    auto* level = new Level();

    std::cout << "Level created" << std::endl;

    std::cout << "Creating environment" << std::endl;

    level->environment = Environment::Earth();

    std::cout << "Environment created" << std::endl;

    std::cout << "Adding objects" << std::endl;

    Cube* cube = new Cube(10.0f);

    cube->canMove = false;
    cube->position = glm::vec3(0.0f, 0.0f, -20.0f);
    cube->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    cube->applyGravity = false;

    level->addObject(*cube);

//    Camera::getActiveInstance()->setFollowObject(cube);


    std::cout << "Objects added" << std::endl;

    return level;
}

void Level::addObject(const Object &object) {

    objects[object.modelType].push_back(object);

}
