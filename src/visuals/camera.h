//
// Created by ofaru on 13.03.2024.
//

#ifndef CAMERA_H
#define CAMERA_H


#include "Angel.h"

class Camera {
public:
    Camera() : position(0.0f, 0.0f, 0.0f), yaw(0.0f), pitch(0.0f), speed(0.1f) {}

    vec3 position;
    GLfloat yaw;
    GLfloat pitch;
    float speed;
    float fov = 90.0f;

    void moveX(float amount);

    void moveY(float amount);

    void moveZ(float amount);

    void moveForward(float amount);


    void moveLeft(float amount);
};

#endif //CAMERA_H
