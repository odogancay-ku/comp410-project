//
// Created by ofaru on 24.03.2024.
//

#ifndef SAT_H
#define SAT_H

#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "Object.h"

// Function to calculate projection of a hitbox onto an axis
std::pair<GLfloat, GLfloat> project(const std::vector<glm::vec3> &hitboxVertices, const glm::vec3 &axis) {
    GLfloat min = std::numeric_limits<GLfloat>::max();
    GLfloat max = -std::numeric_limits<GLfloat>::max();

    for (const auto &vertex: hitboxVertices) {
        float projection = glm::dot(vertex, axis);
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }

    return std::make_pair(min, max);
}

bool overlap(const std::pair<GLfloat, GLfloat> &p1, const std::pair<GLfloat, GLfloat> &p2) {
    return p1.second > p2.first && p1.first < p2.second;
}

bool calculateCollision(const std::vector<glm::vec3> &hitboxVertices1, const std::vector<glm::vec3> &hitboxVertices2,
                        glm::vec3 &MTV) {


    float minOverlap = std::numeric_limits<float>::max(); // Initialize the minimum overlap to a large value

    // Calculate face normals for the first hitbox
    std::vector<glm::vec3> faceNormals1 = {
            glm::normalize(
                    glm::cross(hitboxVertices1[1] - hitboxVertices1[0], hitboxVertices1[2] - hitboxVertices1[0])),
            glm::normalize(
                    glm::cross(hitboxVertices1[3] - hitboxVertices1[0], hitboxVertices1[4] - hitboxVertices1[0])),
            glm::normalize(glm::cross(hitboxVertices1[4] - hitboxVertices1[0], hitboxVertices1[5] - hitboxVertices1[0]))
    };

    // Calculate face normals for the second hitbox
    std::vector<glm::vec3> faceNormals2 = {
            glm::normalize(
                    glm::cross(hitboxVertices2[1] - hitboxVertices2[0], hitboxVertices2[2] - hitboxVertices2[0])),
            glm::normalize(
                    glm::cross(hitboxVertices2[3] - hitboxVertices2[0], hitboxVertices2[4] - hitboxVertices2[0])),
            glm::normalize(glm::cross(hitboxVertices2[4] - hitboxVertices2[0], hitboxVertices2[5] - hitboxVertices2[0]))
    };



    // Add face normals of the first hitbox
    for (const auto &normal: faceNormals1) {
        auto projection1 = project(hitboxVertices1, normal);
        auto projection2 = project(hitboxVertices2, normal);


        // Check for overlap along each axis
        if (!overlap(projection1, projection2)) {
            MTV = glm::vec3(0.0f); // No overlap found, return zero vector as MTV
            return false;
        }

        // Calculate overlap
        float overlap =
                std::min(projection1.second, projection2.second) - std::max(projection1.first, projection2.first);






        // Update minimum overlap and corresponding MTV
        if (overlap < minOverlap) {
            minOverlap = overlap;
            MTV = normal * overlap;
        }
    }

    // Add face normals of the second hitbox
    for (const auto &normal: faceNormals2) {
        auto projection1 = project(hitboxVertices1, normal);
        auto projection2 = project(hitboxVertices2, normal);


        // Check for overlap along each axis
        if (!overlap(projection1, projection2)) {
            MTV = glm::vec3(0.0f); // No overlap found, return zero vector as MTV
            return false;
        }

        // Calculate overlap
        float overlap =
                std::min(projection1.second, projection2.second) - std::max(projection1.first, projection2.first);







        // Update minimum overlap and corresponding MTV
        if (overlap < minOverlap) {
            minOverlap = overlap;
            MTV = normal * overlap;
        }
    }

    // Calculate dot products of face normals of both hitboxes
    for (const auto &normal1: faceNormals1) {
        for (const auto &normal2: faceNormals2) {
            glm::vec3 axis = glm::cross(normal1, normal2); // Axis perpendicular to both face normals
            if (glm::length2(axis) < 0.0001f) {
                continue; // Skip parallel axes
            }

            auto projection1 = project(hitboxVertices1, axis);
            auto projection2 = project(hitboxVertices2, axis);

            // Check for overlap along each axis
            if (!overlap(projection1, projection2)) {
                MTV = glm::vec3(0.0f); // No overlap found, return zero vector as MTV
                return false;
            }

            // Calculate overlap
            float overlap =
                    std::min(projection1.second, projection2.second) - std::max(projection1.first, projection2.first);

            // Update minimum overlap and corresponding MTV
            if (overlap < minOverlap) {
                minOverlap = overlap;
                MTV = axis * overlap;
            }
        }
    }


    return true; // Overlap found along all axes, collision detected
}

void detectAndHandleCollisionSAT(Object *object1, Object *object2) {

    if (!object1->canMove && !object2->canMove) {
        return; // Both objects are static
    }

    std::vector<glm::vec3> hitboxVertices1 = object1->modelData->hitboxVertices;
    std::vector<glm::vec3> hitboxVertices2 = object2->modelData->hitboxVertices;

    for (auto &i: hitboxVertices1) {
        i = glm::vec3(object1->getModelMatrix() * glm::vec4(i, 1.0f));
    }

    for (auto &i: hitboxVertices2) {
        i = glm::vec3(object2->getModelMatrix() * glm::vec4(i, 1.0f));
    }

    glm::vec3 MTV;
    if (calculateCollision(hitboxVertices1, hitboxVertices2, MTV)) {

        object1->onCollision(MTV, object2);
        object2->onCollision(-MTV, object1);

        if (!object1->applyPhysics || !object2->applyPhysics) {
            return; // At least one object cannot collide
        }

        glm::vec3 normalMTV = glm::normalize(MTV);

        // Check which object is on the left side of the MTV

        if (glm::dot(object1->position - object2->position, normalMTV) < 0) {
            normalMTV = -normalMTV;
            MTV = -MTV;
        }

        // Calculate the relative velocity of the two objects
        glm::vec3 relativeVelocity = object1->velocity - object2->velocity;

        if (!object1->canMove) {
            relativeVelocity = -object2->velocity;
        } else if (!object2->canMove) {
            relativeVelocity = object1->velocity;
        }

        // Calculate the relative velocity along the minimum translation vector
        float velocityAlongMTV = glm::dot(relativeVelocity, normalMTV);

        // Calculate the coefficient of restitution
        float e = (object1->restitution+ object2->restitution)/2;

        if (!object1->canMove) {
            object2->position -= MTV;
            // Reflect
            if (velocityAlongMTV >= 0) {
                return; // Objects are moving away from each other
            }
            object2->velocity = glm::reflect(object2->velocity, normalMTV)*e;
            return;
        }

        if (!object2->canMove) {
            object1->position += MTV;
            // Reflect
            if (velocityAlongMTV >= 0) {
                return; // Objects are moving away from each other
            }
            object1->velocity = glm::reflect(object1->velocity, normalMTV)*e;
            return;
        }


        // Calculate the total mass of the two objects
        float totalMass = object1->mass + object2->mass;

        // Calculate the ratio of the masses
        float ratio1 = object2->mass / totalMass;
        float ratio2 = object1->mass / totalMass;


        // Move the objects apart by the minimum translation vector
        object1->position += MTV * ratio1;
        object2->position -= MTV * ratio2;

        if (velocityAlongMTV >= 0) {
            return; // Objects are moving away from each other
        }

        // Calculate the impulse
        float j = -(1 + e) * velocityAlongMTV;
        j /= (1 / object1->mass) + (1 / object2->mass);

        // Apply the impulse to the objects
        glm::vec3 impulse = normalMTV * j;
        object1->velocity += impulse * (1/object1->mass);
        object2->velocity -= impulse * (1/object2->mass);


    }
}


#endif //SAT_H
