//
// Created by ofaru on 16.03.2024.
//

#ifndef LEVEL_H
#define LEVEL_H


#include <vector>
#include <map>
#include "game/persistent/environment/Environment.h"
#include "objects/graphics/ResourceManager.h"
#include "objects/physics/Object.h"

class Level {

public:

    virtual ~Level() {
        std::cout << "Level created" << std::endl;

        objects = std::map<ModelTypes, std::vector<Object*>>();
    }


    Environment *environment;

    std::map<ModelTypes, std::vector<Object*>> objects;


    void addObject(Object* object);


    void setup();

};


#endif //LEVEL_H
