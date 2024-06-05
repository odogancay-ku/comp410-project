
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

glm::vec3 Camera::originalPosition = glm::vec3(0, 0, 0);

Camera::Camera() {
    // Get next id
    instances.push_back(*this);

    movingForward = false;
    movingBackward = false;
    movingLeft = false;
    movingRight = false;
    movingDown = false;
    movingUp = false;

    position = originalPosition;
    velocity = glm::vec3(0, 0, 0);
    canMove = true;

    lastDirectionForward = false;


}

void Camera::updateOrientation(GLfloat yaw, GLfloat pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
}

void Camera::update(GLfloat dt) {

    velocity = glm::vec3(0, 0, 0);


    if (lastDirectionForward && movingForward) {
        speed += speed * dt;
    } else {
        speed = originalSpeed;
    }

    if (movingForward) {
        lastDirectionForward = true;
        velocity += glm::vec3(cos(glm::radians(yaw)), 0, sin(glm::radians(yaw)));
    } else {
        lastDirectionForward = false;
    }

    if (movingBackward) {
        velocity -= glm::vec3(cos(glm::radians(yaw)), 0, sin(glm::radians(yaw)));
    }

    if (movingRight) {
        velocity -= glm::vec3(sin(glm::radians(yaw)), 0, -cos(glm::radians(yaw)));
    }

    if (movingLeft) {
        velocity += glm::vec3(sin(glm::radians(yaw)), 0, -cos(glm::radians(yaw)));
    }

    if (movingUp) {
        velocity += glm::vec3(0, 1, 0);
    }

    if (movingDown) {
        velocity -= glm::vec3(0, 1, 0);
    }

    if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0) {


        velocity = glm::normalize(velocity) * speed;
    }

    position += velocity * dt;

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
    fov = originalFov;


}

void Camera::updateFOV(GLfloat fov) {
    this->fov = fov;
}

void Camera::offsetFOV(GLfloat fov) {
    this->fov += fov;

    if (this->fov > 145.0f) {
        this->fov = 145.0f;
    } else if (this->fov < 45.0f) {
        this->fov = 45.0f;
    }

}

void Camera::offsetOrientation(GLfloat _yaw, GLfloat _pitch) {

    this->yaw += _yaw * sensitivity;
    this->pitch += _pitch * sensitivity;

    if (this->pitch > 89.0f) {
        this->pitch = 89.0f;
    } else if (this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }

    // Ensure yaw stays within [0, 360) degrees
    while (this->yaw >= 360.0f) {
        this->yaw -= 360.0f;
    }
    while (this->yaw < -360.0f) {
        this->yaw += 360.0f;
    }

}

void Camera::setActiveInstance(Camera *pCamera) {
    activeInstance = pCamera;
}


glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                                      sin(glm::radians(pitch)),
                                                      sin(glm::radians(yaw)) * cos(glm::radians(pitch))), glm::vec3(0, 1, 0));

}
