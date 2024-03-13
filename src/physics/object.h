//
// Created by ofaru on 12.03.2024.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "Angel.h"
#include <vector>

class Object {

public:
    static int nextId;

    int id;

    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    vec3 rotation;
    vec3 angularVelocity;
    vec3 angularAcceleration;
    float momentOfInertia;
    float mass;
    float restitution;
    float friction;
    bool isStatic;
    bool isColliding;
    bool isAffectedByGravity;
    bool isSurface;

    std::vector<vec3> vertices = {};
    std::vector<vec3> normals = {};
    std::vector<vec3> colors= {};
    std::vector<GLuint> indices = {};

    std::vector<vec3> hitBoxVertices = {};



    Object(): id(nextId++) {
    }

    void update(float dt);

    void handleCollision(Object &other, vec3 collisionPoint);

    bool checkCollision(Object &other);

    vec3 calculateCollisionPoint(Object &other);

};


class Cube : public Object {
public:
    Cube(vec3 position, vec3 color, float sideLength) {
        this->position = position;
        this->mass = 1.0f; // You can adjust this value
        this->restitution = 0.8f; // You can adjust this value
        this->friction = 0.8f; // You can adjust this value
        this->isStatic = false;
        this->isAffectedByGravity = true;
        this->isSurface = false;
        this->isColliding = true;

        vertices = {
            vec3(-sideLength/2, -sideLength/2, -sideLength/2),
            vec3(sideLength/2, -sideLength/2, -sideLength/2),
            vec3(sideLength/2, sideLength/2, -sideLength/2),
            vec3(-sideLength/2, sideLength/2, -sideLength/2),
            vec3(-sideLength/2, -sideLength/2, sideLength/2),
            vec3(sideLength/2, -sideLength/2, sideLength/2),
            vec3(sideLength/2, sideLength/2, sideLength/2),
            vec3(-sideLength/2, sideLength/2, sideLength/2)

        };

        normals = {
            vec3(0, 0, -1),
            vec3(0, 0, 1),
            vec3(1, 0, 0),
            vec3(-1, 0, 0),
            vec3(0, 1, 0),
            vec3(0, -1, 0)
        };

        colors = std::vector<vec3>(8, color);

        indices = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            0, 4, 7, 0, 7, 3,
            1, 5, 6, 1, 6, 2,
            0, 1, 5, 0, 5, 4,
            3, 2, 6, 3, 6, 7
        };

        hitBoxVertices = {
            vec3(-sideLength/2, -sideLength/2, -sideLength/2),
            vec3(sideLength/2, sideLength/2, sideLength/2),
        };
    }
};


#endif //OBJECT_H
