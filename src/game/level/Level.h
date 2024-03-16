//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_LEVEL_H
#define HOMEWORK1_LEVEL_H


#include <vector>
#include <map>
#include "Environment.h"
#include "../../objects/graphics/ResourceManager.h"
#include "../../objects/physics/Object.h"

class Level {

public:

    Level() {
        std::cout << "Level created" << std::endl;

        objects = std::map<ModelTypes, std::vector<Object>>();
    }


    Environment environment;

    std::map<ModelTypes, std::vector<Object>> objects;


    void addObject(const Object &object);


    static Level* HW1();


};


#endif //HOMEWORK1_LEVEL_H
