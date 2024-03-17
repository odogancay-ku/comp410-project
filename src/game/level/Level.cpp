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



    // Generate 100 spheres on a grid with 10 unit spaces

    for (int i = -5; i < 10; i++) {
        for (int j = -5; j < 10; j++) {

            auto* sphere = new Sphere(5.0f);

            sphere->position = glm::vec3(i * 10.0f, -10.0f, j * 10.0f);
            sphere->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            sphere->applyGravity = false;
            sphere->canMove = false;
            sphere->applyDrag = false;
            sphere->angularVelocity = glm::vec3(15.0f, 15.0f, 15.0f);
            level->addObject(*sphere);

        }
    }

//    Camera::getActiveInstance()->setFollowObject(cube);


    std::cout << "Objects added" << std::endl;

    return level;
}

void Level::addObject(const Object &object) {

    objects[object.modelType].push_back(object);

}
