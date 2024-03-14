//
// Created by ofaru on 12.03.2024.
//

#include <vector>
#include <algorithm>
#include "object.h"

int Object::nextId = 0;

void Object::update(float dt) {
    if (!isStatic) {
        if (isAffectedByGravity) {
            velocity += vec3(0, -9.81, 0) * dt;
        }
        velocity += acceleration * dt;
        position += velocity * dt;
        angularVelocity += angularAcceleration * dt;
        rotation += angularVelocity * dt;
    }
}

void Object::handleCollision(Object &other, vec3 collisionPoint) {

    if (isStatic || other.isStatic) {
        Object &staticObject = isStatic ? *this : other;
        Object &dynamicObject = isStatic ? other : *this;


        // Separate the objects by calculating the closest side of the static object to the collision point
        vec3 min = staticObject.position + staticObject.hitBoxVertices[0];
        vec3 max = staticObject.position + staticObject.hitBoxVertices[1];

        vec3 min2 = dynamicObject.position + dynamicObject.hitBoxVertices[0];
        vec3 max2 = dynamicObject.position + dynamicObject.hitBoxVertices[1];

        // Calculate the overlap in all three dimensions

        float overlapX = std::min(max.x, max2.x) - std::max(min.x, min2.x);
        float overlapY = std::min(max.y, max2.y) - std::max(min.y, min2.y);
        float overlapZ = std::min(max.z, max2.z) - std::max(min.z, min2.z);

        // Find the smallest overlap
        float smallestOverlap = std::min(overlapX, std::min(overlapY, overlapZ));
        // Find the smallest overlap direction
        vec3 separationVector = vec3(0, 0, 0);
        if (smallestOverlap == overlapX) {
            separationVector.x = overlapX;
        } else if (smallestOverlap == overlapY) {
            separationVector.y = overlapY;
        } else {
            separationVector.z = overlapZ;
        }

        vec3 normal = normalize(dynamicObject.position - collisionPoint);


        vec3 relativeVelocity = dynamicObject.velocity;

        float velocityAlongNormal = dot(relativeVelocity, normal);

        if (velocityAlongNormal > 0) {
            return;
        }


        float e = dynamicObject.restitution;


        dynamicObject.velocity -= 2 * velocityAlongNormal * normal;
        dynamicObject.velocity *= e;

        dynamicObject.position += separationVector;





        return;

    }




    vec3 relativeVelocity = velocity - other.velocity;
    vec3 collisionNormal = normalize(position - other.position);

    if (relativeVelocity.x != relativeVelocity.x) {
        // I don't know why this happens, but it does
        // I'm just going to ignore it
        // I'm sorry
        // I'm so sorry
        // I failed you
        // I failed you all
        std::cout << "ERROR: Relative velocity is NaN" << std::endl;
        exit(1);
    }

    float velocityAlongNormal = dot(relativeVelocity, collisionNormal);


    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) {
        return;
    }

    float e = std::min(restitution, other.restitution);

    float j = -(1 + e) * velocityAlongNormal;
    j /= 1 / mass + 1 / other.mass;

    vec3 impulse = j * collisionNormal;

    velocity += (1 / mass) * impulse;
    other.velocity -= (1 / other.mass) * impulse;



//    // Calculate the torque
//    vec3 radius = collisionPoint - position;
//    vec3 torque = cross(radius, impulse);
////
////    // Update the angular velocity
////    angularVelocity += torque / momentOfInertia;

}


bool Object::checkCollision(Object &other) {


    vec3 min1 = position + hitBoxVertices[0];
    vec3 max1 = position + hitBoxVertices[1];

    vec3 min2 = other.position + other.hitBoxVertices[0];
    vec3 max2 = other.position + other.hitBoxVertices[1];

    // Check for overlap in all three dimensions
    if (max1.x < min2.x || min1.x > max2.x) return false;
    if (max1.y < min2.y || min1.y > max2.y) return false;
    if (max1.z < min2.z || min1.z > max2.z) return false;

    // If there is overlap in all three dimensions, the objects are colliding
    return true;
}

vec3 Object::calculateCollisionPoint(Object &other) {
    vec3 min1 = position + hitBoxVertices[0];
    vec3 max1 = position + hitBoxVertices[1];

    vec3 min2 = other.position + other.hitBoxVertices[0];
    vec3 max2 = other.position + other.hitBoxVertices[1];

    // Calculate the centers of the overlapping regions
    float centerX = (std::max(min1.x, min2.x) + std::min(max1.x, max2.x)) / 2.0f;
    float centerY = (std::max(min1.y, min2.y) + std::min(max1.y, max2.y)) / 2.0f;
    float centerZ = (std::max(min1.z, min2.z) + std::min(max1.z, max2.z)) / 2.0f;

    // The collision point is the center of the overlapping region
    vec3 collisionPoint(centerX, centerY, centerZ);

    return collisionPoint;
}

// Helper function to find the normal of a triangle given three points
vec3 findTriangleNormal(const vec3 &p1, const vec3 &p2, const vec3 &p3) {
    return normalize(cross(p2 - p1, p3 - p1));
}

// Helper function to check if two line segments intersect
bool segmentsIntersect(const vec3 &p1, const vec3 &q1, const vec3 &p2, const vec3 &q2) {
    vec3 u = q1 - p1;
    vec3 v = q2 - p2;
    vec3 w = p1 - p2;
    float denominator = dot(u, v) * dot(u, v) - dot(u, u) * dot(v, v);

    // Check if segments are parallel
    if (abs(denominator) < 0.0001) {
        return false;
    }

    float s = (dot(u, v) * dot(w, v) - dot(v, v) * dot(w, u)) / denominator;
    float t = (dot(u, v) * dot(w, u) - dot(u, u) * dot(w, v)) / denominator;

    // Check if intersection point lies within both segments
    return (s >= 0 && s <= 1 && t >= 0 && t <= 1);
}

bool checkFaceIntersection(const std::vector<vec3> &triangle1, const std::vector<vec3> &triangle2) {

    vec3 normal1 = findTriangleNormal(triangle1[0], triangle1[1], triangle1[2]);
    vec3 normal2 = findTriangleNormal(triangle2[0], triangle2[1], triangle2[2]);

    // Check if the planes are parallel
    if (abs(dot(normal1, normal2)) > 0.999) {
        return false; // Planes are parallel, triangles cannot intersect
    }

    // Find the intersection line of the two planes
    vec3 intersectionDir = cross(normal1, normal2);

    // Check if the intersection line intersects both triangles
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (segmentsIntersect(triangle1[i], triangle1[(i + 1) % 3], triangle2[j], triangle2[(j + 1) % 3])) {
                return true; // Intersection found
            }
        }
    }

    return false; // No intersection found

}



