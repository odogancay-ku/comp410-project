//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include "Object.h"
#include "../../game/persistent/level/Level.h"

int Object::nextId = 0;


Object::Object() {
    id = nextId++;
}

void Object::update(GLfloat dt, Environment *environment) {

    // Dynamic updates

    if (applyGravity) {
        if (environment->gravityFunction != nullptr) {

            glm::vec3 gravity = environment->gravityFunction(this->position);
            velocity += dt * gravity;
        }
    }

    if (canMove) {
        velocity += acceleration * dt;
    }

    if (canRotate) {
        rotation += angularVelocity * dt;
    }

    if (canRotate) {
        angularVelocity += angularAcceleration * dt;
    }

    if (applyDrag) {
        velocity -= velocity * 0.1f * dt;
    }

    if (canMove) {
        position += velocity * dt;
    }

}

glm::mat4 Object::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    model = glm::scale(model, stretch);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return model;
}

double scalarProjection(glm::vec3 point, glm::vec3 normal) {
    return glm::dot(point, normal) / glm::dot(normal, normal);
}

// Function to compute the projection of a box onto an axis
std::pair<double, double> projectBox(std::vector<glm::vec3> box, glm::vec3 axis) {
    double min = scalarProjection(box[0], axis);
    double max = min;

    for (auto vertex: box) {
        double scalar = scalarProjection(vertex, axis);
        if (scalar < min) {
            min = scalar;
        } else if (scalar > max) {
            max = scalar;
        }
    }

    return std::make_pair(min, max);
}

// Function to check if two ranges overlap
std::pair<double, double> rangesOverlap(std::pair<double, double> range1, std::pair<double, double> range2) {
    return { range2.second-range1.first, range1.second - range2.first};
}

struct CollisionData {
    bool collision= true;
    std::vector<std::pair<double,double>> box1AxisRanges;
    std::vector<std::pair<double,double>> box2AxisRanges;
    std::vector<std::pair<double,double>> axisOverlaps;
};

// Function to check if two boxes collide
CollisionData* doesCollide(std::vector<glm::vec3> box1, std::vector<glm::vec3> box2) {
    std::vector<glm::vec3> axes = {glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)};


    auto* collisionData = new CollisionData();

    for (auto axis: axes) {
        auto box1Range = projectBox(box1, axis);
        auto box2Range = projectBox(box2, axis);

        collisionData->box1AxisRanges.push_back(box1Range);
        collisionData->box2AxisRanges.push_back(box2Range);

        std::pair<double, double> overlap = rangesOverlap(box1Range, box2Range);

        if (overlap.first < 0 || overlap.second < 0) {
            collisionData->collision = false;
            return collisionData;
        }

        collisionData->axisOverlaps.push_back(overlap);
    }



    return collisionData;
}

void Object::checkCollision(Object *otherObject) {




    ModelData modelData = *ResourceManager::getModel(modelType);

    std::vector<glm::vec3> vertices = modelData.hitboxVertices;

    // Apply model matrix to vertices

    glm::mat4 modelMatrix = getModelMatrix();

    for (auto &vertex: vertices) {
        glm::vec4 vertex4 = glm::vec4(vertex, 1.0f);
        vertex4 = modelMatrix * vertex4;
        vertex = glm::vec3(vertex4);
    }

    ModelData otherModelData = *ResourceManager::getModel(otherObject->modelType);

    std::vector<glm::vec3> otherVertices = otherModelData.hitboxVertices;

    // Apply model matrix to vertices

    glm::mat4 otherModelMatrix = otherObject->getModelMatrix();

    for (auto &vertex: otherVertices) {
        glm::vec4 vertex4 = glm::vec4(vertex, 1.0f);
        vertex4 = otherModelMatrix * vertex4;
        vertex = glm::vec3(vertex4);
    }

    auto* collision = doesCollide(vertices, otherVertices);


    if (collision->collision) {

        // Detect the collision point
        std::vector<std::pair<double,double>> axisOverlaps = collision->axisOverlaps;
        std::vector<std::pair<double,double>> box1AxisRanges = collision->box1AxisRanges;
        std::vector<std::pair<double,double>> box2AxisRanges = collision->box2AxisRanges;

        // Find the center of the overlaps

        glm::vec3 axisXmid = glm::vec3(0.5f*(box1AxisRanges[0].first + box1AxisRanges[0].second), 0.5f*(box2AxisRanges[0].first + box2AxisRanges[0].second), 0.5f*(box2AxisRanges[0].first + box2AxisRanges[0].second));
        glm::vec3 axisYmid = glm::vec3(0.5f*(box1AxisRanges[1].first + box1AxisRanges[1].second), 0.5f*(box2AxisRanges[1].first + box2AxisRanges[1].second), 0.5f*(box2AxisRanges[1].first + box2AxisRanges[1].second));
        glm::vec3 axisZmid = glm::vec3(0.5f*(box1AxisRanges[2].first + box1AxisRanges[2].second), 0.5f*(box2AxisRanges[2].first + box2AxisRanges[2].second), 0.5f*(box2AxisRanges[2].first + box2AxisRanges[2].second));

        // Find the collision point
        glm::vec3 collisionPoint = glm::vec3(0.5f*(axisXmid.x + axisYmid.x + axisZmid.x), 0.5f*(axisXmid.y + axisYmid.y + axisZmid.y), 0.5f*(axisXmid.z + axisYmid.z + axisZmid.z));

        std::cout << "Collision point: " << collisionPoint.x << " " << collisionPoint.y << " " << collisionPoint.z << std::endl;

        // Detect if one of the objects is static

        // Calculate relative velocities, if the objects are separating don't do anything

        glm::vec3 relativeVelocity = velocity - otherObject->velocity;

        // Check if the objects are separating

        double dotProduct = glm::dot(relativeVelocity, position-otherObject->position);

        std::cout << "Dot product: " << dotProduct << std::endl;


        if (dotProduct < 0) {
            free(collision);
            return;
        }

        if (!canMove && otherObject->canMove) {
            // Move the other object
            otherObject->velocity *=  -1;
        } else if (canMove && !otherObject->canMove) {
            // Move this object
            velocity *= -1;
        } else {
            // Move both objects
            velocity *= -1;
            otherObject->velocity *= -1;

        }

    }

    free(collision);
}



