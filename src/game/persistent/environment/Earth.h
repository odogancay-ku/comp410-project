//
// Created by ofaru on 17.03.2024.
//

#ifndef EARTH_H
#define EARTH_H

#include <iostream>
#include <glm/geometric.hpp>
#include "Environment.h"

class Earth : public Environment {

    public:

    Earth() {
        gravityFunction = earthGravity;
        dragFunction = earthDrag;
    }

    static glm::vec3 earthGravity(glm::vec3 position) {
        // center of the earth at 0,0,0
        // 10 m/s^2 at 100m

//        return {0.0f, 0.0f, 0.0f};
        return {0.0f, -9.8f, 0.0f};


        glm::vec3 direction = -1.0f * glm::normalize(position);

        float g = 10;

        float r = glm::length(position);

        if (r < 30) {
            g *= (r/30);
            return g * direction;
        }


        return g *  (float)pow(30,2) * direction/ (float)pow(r,2);

    }

    static glm::vec3 earthDrag(glm::vec3 velocity) {
        return {0.0f, 0.0f, 0.0f};
        // 0.5 * 1.225 * 0.47 * A * v^2
        // A = 0.1 m^2
        // 1.225 kg/m^3
        // 0.47 drag coefficient
        // 0.5 * 1.225 * 0.47 * 0.1 * v^2
        // 0.030175 * v^2
        return -0.030175f * glm::length(velocity) * velocity;
    }
};

#endif //EARTH_H
