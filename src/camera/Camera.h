//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_CAMERA_H
#define HOMEWORK1_CAMERA_H


#include <glew.h>
#include <glm/vec3.hpp>
#include <vector>

class Camera {
private:
    static Camera *activeInstance;

    Camera();



    constexpr static GLfloat originalYaw = 0.0f;
    constexpr static GLfloat originalPitch = 0.0f;
    static glm::vec3 originalPosition;
    constexpr static GLfloat originalSpeed = 5.0f;
    constexpr static GLfloat originalFov = 90.0f;

    GLfloat fov;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat speed;
    GLfloat sensitivity = 1.0f;
    glm::vec3 position;

    bool canMove;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool movingUp;
    bool movingDown;

public:
    static Camera *getActiveInstance() {
        if (activeInstance == nullptr) {
            activeInstance = new Camera();
        }
        return activeInstance;
    };

    void updateOrientation(GLfloat yaw, GLfloat pitch);

    void offsetOrientation(GLfloat yaw, GLfloat pitch);

    void updateFOV(GLfloat fov);

    void update(GLfloat dt);

    void reset();

    void setMovingForward(bool movingForward);

    void setMovingBackward(bool movingBackward);

    void setMovingLeft(bool movingLeft);

    void setMovingRight(bool movingRight);

    void setMovingUp(bool movingUp);

    void setMovingDown(bool movingDown);

    static void setActiveInstance(int id) {
        Camera::activeInstance = &instances[id];
    }

    void offsetFOV(GLfloat fov);

    static std::vector<Camera> instances;
};


#endif //HOMEWORK1_CAMERA_H
