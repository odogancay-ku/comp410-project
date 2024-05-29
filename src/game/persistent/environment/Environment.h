
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#include <glew.h>
#include <glm/vec3.hpp>
#include <functional>

class Environment {


public:


    // Function to calculate gravity for any given glm::vec3
    std::function<glm::vec3(glm::vec3)> gravityFunction;

    std::function<glm::vec3(glm::vec3)> windFunction;

    std::function<glm::vec3(glm::vec3)> dragFunction;


};


#endif //ENVIRONMENT_H
