
#ifndef OBJECT_H
#define OBJECT_H


#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include "objects/graphics/ResourceManager.h"
#include "game/persistent/environment/Environment.h"

class Object {

public:

    static int nextId;

    int id;

    bool canMove = true;
    bool canRotate = true;
    bool canCollide = true;

    bool applyGravity = true;
    bool applyDrag = false;
    bool applyFriction = true;
    bool applyPhysics = true;

    GLfloat scale = 1.0f;
    glm::vec3 stretch = glm::vec3(1.0f, 1.0f, 1.0f);

    GLfloat mass = 1.0f;
    GLfloat restitution = 1.0f;
    GLfloat friction = 0.2f;

    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    Object();

    void update(GLfloat dt, Environment *environment);

    glm::vec3 color;
    bool isHidden = false;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 getModelMatrix();
    glm::mat4 getRotationMatrix();

    void checkCollision(Object *pObject);

    void paint(float x, float y, float z);

    std::function<void(glm::vec3, Object *)> onCollision = [](glm::vec3 normal, Object *pObject) {
    };

    void setOnCollision(std::function<void(glm::vec3, Object *)> callback);

    void rotateAroundPointAndAxis(glm::vec3 point, glm::vec3 axis, float angle);

    float getPitch();

    float getYaw();

    glm::quat static pitchYawRollToQuat(glm::vec3 pitchYawRoll);

    glm::vec3 static quatToPitchYawRoll(glm::quat quat);

};


#endif //OBJECT_H
