//
// Created by ofaru on 16.03.2024.
//

#include "Environment.h"


glm::vec3 earthGravity(glm::vec3 position) {
    return glm::vec3(0.0f, -9.81f, 0.0f);
}

Environment Environment::Earth() {


    Environment environment;
    environment.gravityFunction = earthGravity;

    return environment;

}
