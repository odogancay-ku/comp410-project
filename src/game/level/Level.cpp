//
// Created by ofaru on 16.03.2024.
//

#include "Level.h"
#include "../../objects/physics/Sphere.h"


Level Level::HW1() {

    Level level;

    level.environment = Environment::Earth();

    Sphere sphere = Sphere(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    level.addObject(sphere);


}

void Level::addObject(const Object &object) {

    objects[object.modelType].push_back(object);

}
