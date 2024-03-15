//
// Created by ofaru on 12.03.2024.
//

#include <vector>
#include <algorithm>
#include <fstream>
#include "object.h"

int Object::nextId = 0;

void Object::update(float dt) {
    if (!isStatic) {
        if (isAffectedByGravity) {
            velocity += vec3(0, -9.81, 0) * dt;
        }
        if (applyAcceleration) {
            velocity += acceleration * dt;
        }
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

        vec3 dynamicObjectCenter = (min2 + max2) / 2.0f;

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

        vec3 normal = normalize(dynamicObjectCenter - collisionPoint);


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


void loadOffModel(const std::string &filename, std::vector<vec3> &vertices, std::vector<vec3> &normals,
                  std::vector<vec3> &colors, std::vector<GLuint> &indices, std::vector<vec3> &hitboxVertices,
                  float scale, vec3 color) {

    // Confirm the file is off file by checking the first line
    std::ifstream file(filename);
    std::string firstLine;
    std::getline(file, firstLine);
    if (firstLine.compare("OFF") == 0) {
        std::cout << firstLine << std::endl;
        std::cout << "ERROR: File is not in OFF format" << std::endl;
        exit(1);
    }

    // Read the number of vertices, faces, and edges

    int numVertices, numFaces, numEdges;
    file >> numVertices >> numFaces >> numEdges;


    // Read the vertices
    for (int i = 0; i < numVertices; ++i) {
        float x, y, z;
        file >> x >> y >> z;

        vertices.push_back(vec3(x, y, z));
        colors.push_back({0.8f, 0.8f, 0.8f});
//        colors.push_back(vec3{rand()%100/100.0f, rand()%100/100.0f, rand()%100/100.0f});
    }

    // Read the faces
    for (int i = 0; i < numFaces; ++i) {
        int numSides;
        file >> numSides;
        for (int j = 0; j < numSides; ++j) {
            GLuint index;
            file >> index;
            indices.push_back(index);
        }
    }


    file.close();

    // Rotate the object 90 degrees around the x axis
    for (size_t i = 0; i < vertices.size(); ++i) {
        float y = vertices[i].y;
        float z = vertices[i].z;
        vertices[i].y = z;
        vertices[i].z = -y;
    }


    // Calculate the bounding box of the object, it will be used for collision detection
    float minX = vertices[0].x;
    float minY = vertices[0].y;
    float minZ = vertices[0].z;
    float maxX = vertices[0].x;
    float maxY = vertices[0].y;
    float maxZ = vertices[0].z;

    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].z < minZ) minZ = vertices[i].z;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
        if (vertices[i].z > maxZ) maxZ = vertices[i].z;
    }

    // Calculate the vectoral distance between the min and max points
    vec3 min = vec3(minX, minY, minZ);
    vec3 max = vec3(maxX, maxY, maxZ);

    vec3 distance = max - min;
    float lenDistance = length(distance);

    // Calculate the ratio of the distance to the scale
    float scaleRatio = scale / lenDistance;

    vec3 scaledMin = min * scaleRatio;
    vec3 scaledMax = max * scaleRatio;

    // Iterate over indices and normals and scale them

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] *= scaleRatio;
        normals.push_back(normalize(vertices[i]));
    }

    for (size_t i = 0; i < normals.size(); ++i) {
        normals[i] *= scaleRatio;
    }

    hitboxVertices.push_back(scaledMin);
    hitboxVertices.push_back(scaledMax);
}


