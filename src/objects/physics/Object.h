//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_OBJECT_H
#define HOMEWORK1_OBJECT_H


#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include "../graphics/ResourceManager.h"
#include "../../game/persistent/environment/Environment.h"

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

    GLfloat scale = 1.0f;
    glm::vec3 stretch = glm::vec3(1.0f, 1.0f, 1.0f);

    GLfloat mass = 1.0f;
    GLfloat restitution = 1.0f;
    GLfloat friction = 0.2f;

    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    Object();


    void update(GLfloat dt,  Environment *environment);

    ModelTypes modelType = ModelTypes::CUBE;
    bool isHidden = false;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 getModelMatrix();

    void checkCollision(Object *pObject);

};


#endif //HOMEWORK1_OBJECT_H
