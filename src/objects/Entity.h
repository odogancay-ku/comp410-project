//
// Created by ofarukdogancay on 6/5/24.
//

#ifndef COMP410_PROJECT_ENTITY_H
#define COMP410_PROJECT_ENTITY_H

#include "renderer/Shader.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <cassert>
#include "objects/physics/Collider.h"
#include "physics/sat.h"
#include "objects/graphics/Model.h"

class Entity {
public:
    explicit Entity(size_t id) : id(id) {
    }
    size_t getId() const { return id; }

    size_t id;

    bool isKinematic = false;

    float mass = 1.0f;
    float restitution = 1.0f;
    glm::vec3 velocity = glm::vec3(0.0f);

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 position = glm::vec3(0.0f);

    std::shared_ptr<Model> model = nullptr;
    std::shared_ptr<Hitbox> hitbox = nullptr;
    std::vector<std::shared_ptr<Entity>> children;

    void update(GLfloat dt) {
        for (auto& child : children) {
            child->update(dt);
        }
    }

    void onCollision(glm::vec3 MTV, Entity *collidedEntity) {

    }

    glm::mat4 getTransform() {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = transform * glm::mat4_cast(rotation);
        transform = glm::scale(transform, scale);
        return transform;
    }

};

class Scene {
private:
    std::vector<Entity*> entities;

public:

    Entity* createEntity();

    void update(GLfloat dt);

    void draw(Shader& shader);

};


#endif //COMP410_PROJECT_ENTITY_H
