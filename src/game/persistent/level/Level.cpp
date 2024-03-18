//
// Created by ofaru on 16.03.2024.
//

#include "Level.h"
#include "../../../objects/graphics/ResourceManager.h"
#include "../../../camera/Camera.h"
#include "../environment/Earth.h"


void Level::addObject(Object* object) {

    objects[object->modelType].push_back(object);

}
