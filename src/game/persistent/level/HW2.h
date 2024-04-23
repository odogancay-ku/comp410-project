#ifndef HW2_H
#define HW2_H

#include <iostream>
#include "objects/physics/Object.h"
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"
#include "objects/physics/RubiksCube.h"

class HW2 : public Level {

private:
    RubiksCube* rubiksCube;
    Object* collisionStick;

public:
    void setup() override;

    void onUpdate(float dt) override;

};


#endif //HW2_H
