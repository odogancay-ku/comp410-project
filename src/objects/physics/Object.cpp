//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include "Object.h"
#include "Sphere.h"


Object::Object() {
}

void Object::update(GLfloat dt) {

    // Dynamic updates

    if (applyGravity) {
        velocity += glm::vec3(0.0f, -9.81f, 0.0f) * dt;
    }

    if (canRotate) {
        rotation += angularVelocity * dt;
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


