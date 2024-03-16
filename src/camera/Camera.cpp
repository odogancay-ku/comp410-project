//
// Created by ofaru on 16.03.2024.
//

#include <complex>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include "Camera.h"

glm::vec3 Camera::originalPosition = glm::vec3(0.0f, 0.0f, 0.0f);

Camera::Camera() {
    // Get next id
    instances.push_back(*this);

    position = glm::vec3{0.0f, 0.0f, 0.0f};
    movingForward = false;
    movingBackward = false;
    movingLeft = false;
    movingRight = false;
    movingDown = false;
    movingUp = false;

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

    if (this->pitch > 89.0f) {
        this->pitch = 89.0f;
    } else if (this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }

    if (this->yaw > 360.0f) {
        this->yaw = 0.0f;
    } else if (this->yaw < 0.0f) {
        this->yaw = 360.0f;
    }

}

void Camera::setActiveInstance(Camera *pCamera) {
    activeInstance = pCamera;
}

void Camera::getViewMatrix(glm::mat4 &viewMatrix) {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = normalize(direction);

    // Calculate the right and up vector
    glm::vec3 right = normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = normalize(cross(right, direction));

    glm::vec3 glmPosition = glm::vec3(position.x, position.y, position.z);
    glm::vec3 glmDirection = glm::vec3(direction.x, direction.y, direction.z);
    glm::vec3 glmUp = glm::vec3(up.x, up.y, up.z);

    // Create the view matrix
    viewMatrix = glm::lookAt(glmPosition, glmPosition + glmDirection, glmUp);
}

