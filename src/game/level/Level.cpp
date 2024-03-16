//
// Created by ofaru on 16.03.2024.
//

#include "Level.h"
#include "../../objects/physics/Sphere.h"


Level* Level::HW1() {

    std::cout << "Creating level HW1" << std::endl;

    auto* level = new Level();

    std::cout << "Level created" << std::endl;

    std::cout << "Creating environment" << std::endl;

    level->environment = Environment::Earth();

    std::cout << "Environment created" << std::endl;

    std::cout << "Adding objects" << std::endl;

    Sphere sphere = Sphere(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    sphere.canMove = false;
    sphere.applyGravity = false;

    level->addObject(sphere);

    std::cout << "Objects added" << std::endl;

    return level;
}

void Level::addObject(const Object &object) {

    objects[object.modelType].push_back(object);

}
