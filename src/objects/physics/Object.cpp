//
// Created by ofaru on 16.03.2024.
//

#include <glm/ext/matrix_transform.hpp>
#include "Object.h"
#include "../../game/persistent/level/Level.h"

int Object::nextId = 0;


Object::Object() {
    id = nextId++;
}

void Object::update(GLfloat dt,  Environment *environment) {

    // Dynamic updates

    if (applyGravity) {
        if (environment->gravityFunction != nullptr) {

            glm::vec3 gravity = environment->gravityFunction(this->position);
            velocity += dt*gravity;
        }
    }

    if (canMove) {
        velocity += acceleration * dt;
    }

    if (canRotate) {
        rotation += angularVelocity * dt;
    }

    if (canRotate) {
        angularVelocity += angularAcceleration * dt;
    }

    if (applyDrag) {
        velocity -= velocity * 0.1f * dt;
    }

    if (canMove) {
        position += velocity * dt;
    }

}

glm::mat4 Object::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    model = glm::scale(model, stretch);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return model;
}

double scalarProjection(glm::vec3 point, glm::vec3 normal) {
    return glm::dot(point, normal) / glm::dot(normal, normal);
}


void Object::checkCollision(Object *otherObject) {

// I am assuming normal is a unit vector
// although the overlap algorithm will still
// work even if normal is not a unit vector
//    double scalarProjection(point, normal)
//    {
//// http://en.wikipedia.org/wiki/Vector_projection
//        return point.dot(normal) / normal.dot(normal);
//    }
//    Range projectBox(box, normal)
//    {
//        Range result;
//
//// iterate over each corner in the box
//        foreach (vertex in box.vertices)
//        {
//            double scalar = scalarProjection(vertex, normal);
//
//            if first vertex
//                        {
//                                result.max = scalar;
//                        result.min = scalar;
//                        }
//                    else if (scalar > result.max)
//                result.max = scalar;
//            else if (scalar < result.min)
//                result.min = scalar;
//        }
//    }
//    bool doesCollide(boxa, boxb)
//    {
//// create an array of vectors the represent the combined axis of the two boxes
//        Array<Vector> boxaxislist;
//
//        boxaxislist.append(boxa.axisList);
//        boxaxislist.append(boxb.axisList);
//
//        foreach (axis in boxaxislist)
//        {
//            Range boxaRange = projectBox(boxa, axis);
//            Range boxbRange = projectBox(boxb, axis);
//
//            if (!boxaRange.intersects(boxbRange))
//            {
//// there was no overlap, we found a plane that separates the two boxes.
//                return false;
//            }
//        }
//
//// no plane was found, the boxes must be overlapping
//        return true;
//    }

    std::vector<glm::vec3> vertices = {};
    glm::vec3 normal = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));

    // Create vertices from min and max point
    ModelData* modelData = ResourceManager::getModel(this->modelType);
    glm::vec3 min = modelData->hitbox[0];
    glm::vec3 max = modelData->hitbox[1];

    vertices = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, max.z),
        glm::vec3(min.x, max.y, max.z)
    };

    // Iterate over each corner in the box

    std::vector<double> scalarProjections = {};

    for (auto vertex : vertices) {
        double scalar = scalarProjection(vertex, normal);
        scalarProjections.push_back(scalar);
    }

    std::vector<double> otherScalarProjections = {};

    for (auto vertex : vertices) {
        double scalar = scalarProjection(vertex, normal);
        otherScalarProjections.push_back(scalar);
    }





}


