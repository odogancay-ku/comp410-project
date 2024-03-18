//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_CAMERA_H
#define HOMEWORK1_CAMERA_H


#include <glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "../objects/physics/Object.h"

class Camera {
private:
    static inline Camera *activeInstance = nullptr;

public:
    constexpr static GLfloat originalYaw = 0.0f;
    constexpr static GLfloat originalPitch = 0.0f;
    static glm::vec3 originalPosition;
    constexpr static GLfloat originalSpeed = 7.0f;
    constexpr static GLfloat originalFov = 90.0f;

    GLfloat fov = originalFov;
    GLfloat yaw = originalYaw;
    GLfloat pitch = originalPitch;
    GLfloat speed = originalSpeed;
    GLfloat sensitivity = 1.0f;
    glm::vec3 position;
    glm::vec3 velocity;

    bool canMove;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;
    bool movingUp;
    bool movingDown;

    Object* followObject = nullptr;


    static Camera *getActiveInstance() {
        if (activeInstance == nullptr) {
            exit(1);
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

    void offsetFOV(GLfloat fov);

    inline static std::vector<Camera> instances = std::vector<Camera>();

    Camera();

    static void setActiveInstance(Camera *pCamera);


    void setFollowObject(Object *object);

    bool lastDirectionForward;
};


#endif //HOMEWORK1_CAMERA_H
