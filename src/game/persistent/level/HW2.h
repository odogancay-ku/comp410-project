#ifndef HW2_H
#define HW2_H

#include <iostream>
#include "objects/physics/Object.h"
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"
#include "objects/physics/RubiksCube.h"
#include "objects/physics/RubiksCube2x2.h"

class HW2 : public Level {

private:
    RubiksCube *rubiksCube;
    RubiksCube2x2 *rubiksCube2X2;

    Object *collisionStick;
    Object *hitMark;
    Object *pullMark;
    Object *trackedCube;
    glm::vec3 trackedCubeHoldStartPosition;
    glm::vec3 trackedCubeHoldPosition;
    float trackedCubePullMarkDistance;
    std::vector<std::tuple<glm::vec3, glm::vec3, Object *>> candidateCollisions;

    std::vector<std::pair<glm::vec3, glm::quat>> originalFinalPositions;
    std::vector<std::pair<glm::vec3, glm::quat>> originalStartPositions;
    int rotationState = 0;
    float rotationQueueAnimationTime = 0.0f;
    bool playForward = false;
    bool playBack = false;
    std::vector<std::tuple<int,int>> rotationQueue;
    float playSpeed = 5.0f;
    int shuffleCount = 25;

    std::vector<std::pair<glm::vec3, glm::quat>> originalFinalPositions2x2;
    std::vector<std::pair<glm::vec3, glm::quat>> originalStartPositions2x2;
    int rotationState2x2 = 0;
    float rotationQueueAnimationTime2x2 = 0.0f;
    bool playForward2x2 = false;
    bool playBack2x2 = false;
    std::vector<std::tuple<int,int>> rotationQueue2x2;
    float playSpeed2x2 = 5.0f;
    int shuffleCount2x2 = 8;

public:
    void setup() override;

    void onUpdate(float dt) override;


};


#endif //HW2_H
