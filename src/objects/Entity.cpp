//
// Created by ofarukdogancay on 6/5/24.
//

#include <map>
#include "Entity.h"
#include "renderer/Shader.h"
#include "objects/graphics/Model.h"
#include "camera/Camera.h"
#include "renderer/Renderer.h"

Entity* Scene::createEntity() {
    auto* entity = new Entity(entities.size());
    entities.push_back(entity);
    return entity;
}

void Scene::update(GLfloat dt) {
    Camera::getActiveInstance()->update(dt);

//    for (auto* entity : entities) {
//        entity->update(dt);
//    }
}

void Scene::draw(Shader &shader) {


    Renderer::objectShader->setMat4("view", Camera::getActiveInstance()->getViewMatrix());
    Renderer::objectShader->setVec3("viewPos", Camera::getActiveInstance()->position);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::map<int, std::vector<Entity>> instancedModels;
    for (auto* entity : entities) {
        if (entity->model) {
            if (entity->model->instanceId > 0) {
                instancedModels[entity->model->instanceId].push_back(*entity);
                continue;
            }
            entity->model->draw(shader, entity->getTransform());
        }
    }

    // Draw instanced models



    for (auto& [instanceId, instancedModels] : instancedModels) {
        std::shared_ptr<Model> model = instancedModels[0].model;
        std::vector<glm::mat4> transforms;
        for (auto& model : instancedModels) {
            transforms.push_back(model.getTransform());
        }
        model->drawInstanced(shader, transforms);
    }
}
