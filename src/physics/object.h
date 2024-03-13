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

    Object(): id(nextId++) {}

    void update(float dt);

    void handleCollision(Object &other, vec3 collisionPoint);

    bool checkCollision(Object &other);

    vec3 calculateCollisionPoint(Object &other);
};


class Cube : public Object {

public:
    Cube(vec3 position, float size, float mass, float restitution, float friction, bool isStatic, bool isAffectedByGravity, bool isSurface) {
        this->position = position;
        this->mass = mass;
        this->restitution = restitution;
        this->friction = friction;
        this->isStatic = isStatic;
        this->isAffectedByGravity = isAffectedByGravity;
        this->isSurface = isSurface;

        vertices = {
                vec3(-size, -size, -size),
                vec3(size, -size, -size),
                vec3(size, size, -size),
                vec3(-size, size, -size),
                vec3(-size, -size, size),
                vec3(size, -size, size),
                vec3(size, size, size),
                vec3(-size, size, size)
        };

        normals = {
                vec3(0, 0, -1),
                vec3(0, 0, 1),
                vec3(1, 0, 0),
                vec3(-1, 0, 0),
                vec3(0, 1, 0),
                vec3(0, -1, 0)
        };

        colors = {
                vec3(1, 0, 0),
                vec3(0, 1, 0),
                vec3(0, 0, 1),
                vec3(1, 1, 0),
                vec3(1, 0, 1),
                vec3(0, 1, 1)
        };

        indices = {
                0, 1, 2, 0, 2, 3,
                4, 5, 6, 4, 6, 7,
                0, 4, 7, 0, 7, 3,
                1, 5, 6, 1, 6, 2,
                0, 1, 5, 0, 5, 4,
                3, 2, 6, 3, 6, 7
        };
    }

};


class Surface : public Object {

public:
    Surface(const std::vector<vec3>& vertices, const vec3& color) {
        this->vertices = vertices;
        this->colors = std::vector<vec3>(4, color);
        this->isStatic = true;
        this->isAffectedByGravity = false;
        this->isColliding = true;

        // Assuming the vertices are in a plane, we can calculate the normal
        vec3 edge1 = vertices[1] - vertices[0];
        vec3 edge2 = vertices[2] - vertices[0];
        vec3 normal = cross(edge1, edge2);
        normal = normalize(normal);

        this->normals = std::vector<vec3>(4, normal);

        // Define indices for two triangles that make up the surface
        this->indices = {0, 1, 2, 0, 2, 3};
    }
};

#endif //OBJECT_H
