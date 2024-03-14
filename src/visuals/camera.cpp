//
// Created by ofaru on 13.03.2024.
//

#include <glm/trigonometric.hpp>
#include "camera.h"

void Camera::moveForward(float amount) {

    position.x += amount * speed * cos(glm::radians(yaw));
    position.z += amount * speed * sin(glm::radians(yaw));

}

void Camera::moveLeft(float amount) {
    position.x += amount * speed * sin(glm::radians(yaw));
    position.z -= amount * speed * cos(glm::radians(yaw));

}


void Camera::moveX(float amount) {
    position.x += amount * speed;
}

void Camera::moveY(float amount) {
    position.y += amount * speed;
}

void Camera::moveZ(float amount) {
    position.z += amount * speed;
}


