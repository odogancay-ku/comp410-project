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
    RubiksCube *rubiksCube;
    Object *collisionStick;
    Object *hitMark;
    Object *pullMark;
    Object *trackedCube;
    glm::vec3 trackedCubeHoldStartPosition;
    glm::vec3 trackedCubeHoldPosition;
    float trackedCubePullMarkDistance;
    std::vector<std::tuple<glm::vec3, glm::vec3, Object *>> candidateCollisions;

    float rotationState = 0;

public:
    void setup() override;

    void onUpdate(float dt) override;

};


#endif //HW2_H
