
#ifndef LEVEL_H
#define LEVEL_H


#include <vector>
#include <map>
#include "game/persistent/environment/Environment.h"
#include "objects/graphics/ResourceManager.h"
#include "objects/physics/Object.h"
#include "objects/graphics/Light.h"

class Level {

public:

    virtual ~Level() {

//        objects = std::map<ModelTypes, std::vector<Object*>>();
    }

    Light* light;

    Environment *environment;

//    std::map<ModelTypes, std::vector<Object*>> objects;


    void addObject(Object* object);


    virtual void setup() = 0;

    virtual void onUpdate(float dt) = 0;

};


#endif //LEVEL_H
