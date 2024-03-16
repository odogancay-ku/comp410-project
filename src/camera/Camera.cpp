//
// Created by ofaru on 16.03.2024.
//

#include <complex>
#include "Camera.h"

glm::vec3 Camera::originalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

Camera::Camera() {
    // Get next id
    instances.push_back(*this);

    fov = originalFov;
    yaw = originalYaw;
    pitch = originalPitch;
    speed = originalSpeed;
    position = originalPosition;
    canMove = true;
}

void Camera::updateOrientation(GLfloat yaw, GLfloat pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
}

void Camera::update(GLfloat dt) {

    if (movingForward) {
        position += glm::vec3(cos(yaw), 0, sin(yaw)) * speed * dt;
    }

    if (movingBackward) {
        position -= glm::vec3(cos(yaw), 0, sin(yaw)) * speed * dt;
    }

    if (movingLeft) {
        position -= glm::vec3(sin(yaw), 0, -cos(yaw)) * speed * dt;
    }

    if (movingRight) {
        position += glm::vec3(sin(yaw), 0, -cos(yaw)) * speed * dt;
    }

    if (movingUp) {
        position += glm::vec3(0, 1, 0) * speed * dt;
    }

    if (movingDown) {
        position -= glm::vec3(0, 1, 0) * speed * dt;
    }

}

void Camera::setMovingForward(bool movingForward) {
    Camera::movingForward = movingForward;
}

void Camera::setMovingBackward(bool movingBackward) {
    Camera::movingBackward = movingBackward;
}

void Camera::setMovingLeft(bool movingLeft) {
    Camera::movingLeft = movingLeft;
}

void Camera::setMovingRight(bool movingRight) {
    Camera::movingRight = movingRight;
}

void Camera::setMovingUp(bool movingUp) {
    Camera::movingUp = movingUp;
}

void Camera::setMovingDown(bool movingDown) {
    Camera::movingDown = movingDown;
}

void Camera::reset() {
    movingForward = false;
    movingBackward = false;
    movingLeft = false;
    movingRight = false;
    movingUp = false;
    movingDown = false;

    position = originalPosition;
    yaw = originalYaw;
    pitch = originalPitch;
    speed = originalSpeed;
}

void Camera::updateFOV(GLfloat fov) {
    this->fov = fov;
}

void Camera::offsetFOV(GLfloat fov) {
    this->fov += fov;
}

void Camera::offsetOrientation(GLfloat yaw, GLfloat pitch) {
    this->yaw += yaw * sensitivity;
    this->pitch += pitch * sensitivity;
}

