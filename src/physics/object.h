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

    int type;

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
    bool isHidden = false;
    bool applyAcceleration = true;

    std::vector<vec3> vertices = {};
    std::vector<vec3> normals = {};
    std::vector<vec3> colors = {};
    std::vector<GLuint> indices = {}; // ALWAYS MULTIPLES OF 3

    std::vector<vec3> hitBoxVertices = {};


    Object() : id(nextId++) {
    }

    void update(float dt);

    void handleCollision(Object &other, vec3 collisionPoint);

    bool checkCollision(Object &other);

    vec3 calculateCollisionPoint(Object &other);

};


class Cube : public Object {
public:
    Cube(vec3 position, vec3 color, float sideLength) {
        this->type = 1;
        this->position = position;
        this->mass = sideLength; // You can adjust this value
        this->restitution = 0.8f; // You can adjust this value
        this->friction = 0.8f; // You can adjust this value
        this->isStatic = false;
        this->isAffectedByGravity = true;
        this->isSurface = false;
        this->isColliding = true;

        vertices = {
                vec3(-sideLength / 2, -sideLength / 2, -sideLength / 2),
                vec3(sideLength / 2, -sideLength / 2, -sideLength / 2),
                vec3(sideLength / 2, sideLength / 2, -sideLength / 2),
                vec3(-sideLength / 2, sideLength / 2, -sideLength / 2),
                vec3(-sideLength / 2, -sideLength / 2, sideLength / 2),
                vec3(sideLength / 2, -sideLength / 2, sideLength / 2),
                vec3(sideLength / 2, sideLength / 2, sideLength / 2),
                vec3(-sideLength / 2, sideLength / 2, sideLength / 2)

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
                vec3(-sideLength / 2, -sideLength / 2, -sideLength / 2),
                vec3(sideLength / 2, sideLength / 2, sideLength / 2),
        };
    }
};

class Sphere : public Object {
public:
    Sphere(vec3 position, vec3 color, float radius) {
        this->type = 2;
        this->position = position;
        this->mass = 4.0f / 3.0f * M_PI * pow(radius, 3); // Mass is proportional to the volume
        this->restitution = 0.8f; // You can adjust this value
        this->friction = 0.8f; // You can adjust this value
        this->isStatic = false;
        this->isAffectedByGravity = true;
        this->isSurface = false;
        this->isColliding = true;

        generateVertices(radius, color);

        // The hitbox of the sphere is a box that completely encloses the sphere.
        hitBoxVertices = {
                vec3(-radius, -radius, -radius),
                vec3(radius, radius, radius),
        };
    }

    void generateVertices(float radius, vec3 color, int slices = 36, int stacks = 36) {
        for (int i = 0; i <= stacks; ++i) {
            float phi = static_cast<float>(i) * M_PI / static_cast<float>(stacks);
            for (int j = 0; j <= slices; ++j) {
                float theta = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(slices);
                float x = std::sin(phi) * std::cos(theta);
                float y = std::cos(phi);
                float z = std::sin(phi) * std::sin(theta);
                vertices.push_back(vec3{x, y, z} * radius);
//                colors.push_back(vec3{rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f});
                colors.push_back(color);
                normals.push_back(vec3{x, y, z});
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < slices; ++j) {
                indices.push_back((i + 1) * (slices + 1) + j);
                indices.push_back(i * (slices + 1) + j);
                indices.push_back(i * (slices + 1) + j + 1);

                indices.push_back((i + 1) * (slices + 1) + j);
                indices.push_back(i * (slices + 1) + j + 1);
                indices.push_back((i + 1) * (slices + 1) + j + 1);
            }
        }
    }

};

void loadOffModel(const std::string &filename, std::vector<vec3> &vertices, std::vector<vec3> &normals,
                  std::vector<vec3> &colors, std::vector<GLuint> &indices, std::vector<vec3> &hitboxVertices,
                  float scale, vec3 color);

class Bunny : public Object {

public:
    Bunny(vec3 position, vec3 color, float scale) {
        this->type = 3;
        this->position = position;
        this->mass = scale;
        this->restitution = 0.8f;
        this->friction = 0.8f;
        this->isStatic = false;
        this->isAffectedByGravity = true;
        this->isSurface = false;
        this->isColliding = true;

        std::vector<vec3> vertices;
        std::vector<vec3> normals;
        std::vector<vec3> colors;
        std::vector<GLuint> indices;
        std::vector<vec3> hitBoxVertices;

        loadOffModel("assets/models/bunny.off", vertices, normals, colors, indices, hitBoxVertices, scale, color);

        this->vertices = vertices;
        this->normals = normals;
        this->colors = colors;
        this->indices = indices;
        this->hitBoxVertices = hitBoxVertices;

    }

};


#endif //OBJECT_H
