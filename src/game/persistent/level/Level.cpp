
#include "Level.h"
#include "objects/graphics/ResourceManager.h"
#include "camera/Camera.h"
#include "game/persistent/environment/Earth.h"


void Level::addObject(Object* object) {

    objects[object->modelType].push_back(object);

}
