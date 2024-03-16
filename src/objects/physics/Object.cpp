//
// Created by ofaru on 16.03.2024.
//

#include "Object.h"
#include "Sphere.h"
#include "Player.h"


Object::Object() {
}

void Object::update(GLfloat dt) {

    // Dynamic updates

    if (applyGravity) {
        velocity += glm::vec3(0.0f, -9.81f, 0.0f) * dt;
    }

    if (canMove) {
        position += velocity * dt;
    }

    if (canRotate) {
        rotation += angularVelocity * dt;
    }

    if (applyDrag) {
        velocity -= velocity * 0.1f * dt;
    }

}


