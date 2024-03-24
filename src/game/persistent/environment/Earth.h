//
// Created by ofaru on 17.03.2024.
//

#ifndef HOMEWORK1_EARTH_H
#define HOMEWORK1_EARTH_H

#include <iostream>
#include <glm/geometric.hpp>
#include "Environment.h"

class Earth : public Environment {

    public:

    Earth() {
        std::cout << "Creating environment Earth" << std::endl;
        gravityFunction = earthGravity;
    }

    static glm::vec3 earthGravity(glm::vec3 position) {
        // center of the earth at 0,0,0
        // 10 m/s^2 at 100m

        glm::vec3 direction = -1.0f * glm::normalize(position);

        float g = 10;

        float r = glm::length(position);

        if (r < 30) {
            g *= (r/30);
            return g * direction;
        }

        return glm::vec3(0.0f);

        return g *  (float)pow(30,2) * direction/ (float)pow(r,2);

    }

};

#endif //HOMEWORK1_EARTH_H
