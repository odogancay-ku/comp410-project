//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include "Object.h"
#include "../../game/persistent/level/Level.h"
#include "../../renderer/Renderer.h"

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


        float roomSize = 15.0f;


        if (position.x > 3.0f/2.0f*roomSize-2 && velocity.x > 0) {
            velocity.x *= -1;
        } else if (position.x < roomSize/2 && velocity.x < 0) {
            velocity.x *= -1;
        } else if (position.y > roomSize/2-2 && velocity.y > 0) {
            velocity.y *= -1;
        } else if (position.y < -roomSize/2+2 && velocity.y < 0) {
            velocity.y *= -1;
        } else if (position.z > roomSize/2-2 && velocity.z > 0) {
            velocity.z *= -1;
        } else if (position.z < -roomSize/2+2 && velocity.z < 0) {
            velocity.z *= -1;
        }

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

    // FOR NOW


}



