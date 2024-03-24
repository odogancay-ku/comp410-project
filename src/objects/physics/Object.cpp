//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include "Object.h"
#include "../../game/persistent/level/Level.h"
#include "../../renderer/Renderer.h"
#include "sat.h"

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



void Object::checkCollision(Object *otherObject) {

    detectAndHandleCollisionSAT(this, otherObject);

}



