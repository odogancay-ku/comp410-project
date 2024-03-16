//
// Created by ofaru on 16.03.2024.
//

#include <iostream>
#include "Environment.h"


glm::vec3 earthGravity(glm::vec3 position) {
    return {0.0f, -9.81f, 0.0f};
}

Environment Environment::Earth() {

    std::cout << "Creating environment Earth" << std::endl;

    Environment environment = Environment();
    environment.gravityFunction = earthGravity;

    return environment;

}
