//
// Created by ofaru on 13.03.2024.
//

#include "camera.h"



void Camera::moveX(float amount) {
    position.x += amount * speed;
}

void Camera::moveY(float amount) {
    position.y += amount * speed;
}

void Camera::moveZ(float amount) {
    position.z += amount * speed;
}


