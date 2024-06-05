//
// Created by ofarukdogancay on 6/5/24.
//

#ifndef COMP410_PROJECT_COLLIDER_H
#define COMP410_PROJECT_COLLIDER_H

#include <glm/glm.hpp>
#include <memory>

class Hitbox {
public:
    glm::vec3 localCenter;
    float localRadius;
    glm::vec3 vertices[8]{};
    glm::vec3 normals[3]{};

    Hitbox(glm::vec3 *vertices, glm::vec3 *normals) {
        for (int i = 0; i < 8; i++) {
            this->vertices[i] = vertices[i];
        }
        for (int i = 0; i < 3; i++) {
            this->normals[i] = normals[i];
        }

        localCenter = glm::vec3(0.0f);
        localRadius = 0.0f;

        for (int i = 0; i < 8; i++) {
            localCenter += vertices[i];
        }
        localCenter /= 8.0f;

        // Find the maximum distance from the center to the vertices
        for (int i = 0; i < 8; i++) {
            float distance = glm::length(vertices[i] - localCenter);
            if (distance > localRadius) {
                localRadius = distance;
            }
        }

    }

    static std::shared_ptr<Hitbox> newCubeCollider() {
        glm::vec3 vertices[8] = {
                glm::vec3(-0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, -0.5f, -0.5f),
                glm::vec3(0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, 0.5f, -0.5f),
                glm::vec3(-0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, -0.5f, 0.5f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(-0.5f, 0.5f, 0.5f)
        };
        glm::vec3 normals[3] = {
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        };
        return std::make_shared<Hitbox>(vertices, normals);
    }


};

#endif //COMP410_PROJECT_COLLIDER_H
