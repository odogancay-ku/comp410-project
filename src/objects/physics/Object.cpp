//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include <utility>
#include "Object.h"
#include "game/persistent/level/Level.h"
#include "renderer/Renderer.h"
#include "sat.h"

int Object::nextId = 0;


Object::Object() {
    id = nextId++;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    stretch = glm::vec3(1.0f, 1.0f, 1.0f);
    color = glm::vec3(1.0f);
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
        angularVelocity += angularAcceleration * dt;
    }

    if (applyDrag) {
        if (environment->dragFunction != nullptr) {
            glm::vec3 drag = environment->dragFunction(this->velocity);
            velocity -= drag * dt;
        }
    }

    if (canMove) {
        position += velocity * dt;
    }

}

glm::mat4 Object::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(orientation);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    model = glm::scale(model, stretch);
    return model;
}



void Object::checkCollision(Object *otherObject) {

    detectAndHandleCollisionSAT(this, otherObject);


    if (glm::length2(this->velocity)<0.0001f) {
        this->velocity = glm::vec3(0.0f);
    }
    if (glm::length2(otherObject->velocity)<0.0001f) {
        otherObject->velocity = glm::vec3(0.0f);
    }

}

void Object::paint(float x, float y, float z) {
    this->color.x =x;
    this->color.y =y;
    this->color.z =z;
}

void Object::setOnCollision(std::function<void(glm::vec3,Object *)> callback) {
    this->onCollision = std::move(callback);
}

void Object::rotateAroundPointAndAxis(glm::vec3 point, glm::vec3 axis, float angle) {

    // Translate to origin

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -point);

    // Rotate

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);

    // Translate back

    glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), point);

    glm::mat4 transformation = translationBack * rotation * translation;

    glm::vec4 newPosition = transformation * glm::vec4(position, 1.0f);

    position = glm::vec3(newPosition);

    orientation = glm::normalize(glm::quat_cast(transformation * glm::mat4_cast(orientation)));

}

glm::quat  Object::pitchYawRollToQuat(glm::vec3 pitchYawRoll) {

    glm::quat q = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    q = glm::rotate(q, glm::radians(pitchYawRoll.x), glm::vec3(1.0f, 0.0f, 0.0f));
    q = glm::rotate(q, glm::radians(pitchYawRoll.y), glm::vec3(0.0f, 1.0f, 0.0f));
    q = glm::rotate(q, glm::radians(pitchYawRoll.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return q;
}

glm::vec3  Object::quatToPitchYawRoll(glm::quat quat) {
    glm::vec3 angles = glm::eulerAngles(quat);
    return glm::degrees(angles);
}

float Object::getPitch() {

    return quatToPitchYawRoll(orientation).z;

}

float Object::getYaw() {
    return quatToPitchYawRoll(orientation).y;
}




