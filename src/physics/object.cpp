//
// Created by ofaru on 12.03.2024.
//

#include <vector>
#include "object.h"

int Object::nextId = 0;

void Object::update(float dt) {
    if (!isStatic) {
        if (isAffectedByGravity) {
            acceleration += vec3(0, -9.81, 0);
        }
        velocity += acceleration * dt;
        position += velocity * dt;
        angularVelocity += angularAcceleration * dt;
        rotation += angularVelocity * dt;
    }
}

void Object::handleCollision(Object &other, vec3 collisionPoint) {
    vec3 relativeVelocity = velocity - other.velocity;
    vec3 collisionNormal = normalize(position - other.position);

    float velocityAlongNormal = dot(relativeVelocity, collisionNormal);

    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0)
        return;

    float e = std::min(restitution, other.restitution);

    float j = -(1 + e) * velocityAlongNormal;
    j /= 1 / mass + 1 / other.mass;

    vec3 impulse = j * collisionNormal;

    velocity -= (1 / mass) * impulse;
    other.velocity += (1 / other.mass) * impulse;

    // Calculate the torque
    vec3 radius = collisionPoint - position;
    vec3 torque = cross(radius, impulse);

    // Update the angular velocity
    angularVelocity += torque / momentOfInertia;

}

vec3 Object::calculateCollisionPoint(Object &other) {
    for (int i = 0; i < vertices.size(); i++) {
        vec3 vertex = vertices[i];
        vec3 normal = normals[i];

        vec3 vertexWorldSpace = position + vertex;
        vec3 vertexWorldSpaceOther = other.position + vertex;

        vec3 vertexWorldSpaceToOther = vertexWorldSpace - vertexWorldSpaceOther;

        if (dot(vertexWorldSpaceToOther, normal) < 0) {
            return vertexWorldSpace;
        }
    }
    return vec3(0, 0, 0);
}

bool Object::checkCollision(Object &other) {
    for (int i = 0; i < vertices.size(); i++) {
        vec3 vertex = vertices[i];
        vec3 normal = normals[i];

        vec3 vertexWorldSpace = position + vertex;
        vec3 vertexWorldSpaceOther = other.position + vertex;

        vec3 vertexWorldSpaceToOther = vertexWorldSpace - vertexWorldSpaceOther;

        if (dot(vertexWorldSpaceToOther, normal) < 0) {
            return true;
        }
    }
    return false;
}

