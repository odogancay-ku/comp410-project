//
// Created by ofaru on 12.03.2024.
//

#include <vector>
#include "object.h"


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


void Object::draw() {
    for (int i = 0; i < indices.size(); i += 3) {
        vec3 vertex1 = vertices[indices[i]];
        vec3 vertex2 = vertices[indices[i + 1]];
        vec3 vertex3 = vertices[indices[i + 2]];

        vec3 normal1 = normals[indices[i]];
        vec3 normal2 = normals[indices[i + 1]];
        vec3 normal3 = normals[indices[i + 2]];

        vec3 color1 = colors[indices[i]];
        vec3 color2 = colors[indices[i + 1]];
        vec3 color3 = colors[indices[i + 2]];

        drawTriangle(position + vertex1, position + vertex2, position + vertex3, normal1, normal2, normal3, color1, color2, color3);
    }
}