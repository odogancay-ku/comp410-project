//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_ENVIRONMENT_H
#define HOMEWORK1_ENVIRONMENT_H


#include <glew.h>
#include <glm/vec3.hpp>
#include <functional>
#include "FastNoiseLite.h"

class Environment {


private:

    // Function to calculate gravity for any given glm::vec3
    std::function<glm::vec3(glm::vec3)> gravityFunction;

    std::function<glm::vec3(glm::vec3)> windFunction;

    FastNoiseLite noise;


public:

    // Environments
    static Environment Earth();

};


#endif //HOMEWORK1_ENVIRONMENT_H
