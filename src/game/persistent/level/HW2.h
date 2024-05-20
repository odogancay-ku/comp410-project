#ifndef HW2_H
#define HW2_H

#include <iostream>
#include "objects/physics/Object.h"
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"
#include "objects/physics/RubiksCube.h"
#include "objects/physics/RubiksCube2x2.h"
#include "objects/physics/TightRope.h"
#include "objects/physics/SegmentDisplay.h"

class HW2 : public Level {

private:
    RubiksCube *rubiksCube;
    RubiksCube2x2 *rubiksCube2X2;

    Object *collisionStick;
    Object *hitMark;
    Object *pullMark;
    Object *crossHair;
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
    std::vector<std::tuple<int, int>> rotationQueue;
    float playSpeed = 5.0f;
    int shuffleCount = 25;

    std::vector<std::pair<glm::vec3, glm::quat>> originalFinalPositions2x2;
    std::vector<std::pair<glm::vec3, glm::quat>> originalStartPositions2x2;
    int rotationState2x2 = 0;
    float rotationQueueAnimationTime2x2 = 0.0f;
    bool playForward2x2 = false;
    bool playBack2x2 = false;
    std::vector<std::tuple<int, int>> rotationQueue2x2;
    float playSpeed2x2 = 5.0f;
    int shuffleCount2x2 = 8;

    TightRope *tightRope;


    int segmentDisplayCount = 15;
    SegmentDisplay *segmentDisplays[15];

    int currentMessagePosition = 0;
    float messageDisplayTime = 0.0f;

public:
    void setup() override;

    void onUpdate(float dt) override;

    void generateSegmentDisplays(glm::vec3 position, glm::quat orientation, float scale, float spacing);

    void updateSegmentDisplays();

    void writeStringToSegmentDisplays(std::string str);

    std::string message = "             WELCOME INSTRUCTIONS  PRESS ESC TO EXIT  PRESS W A S D LEFT SHIFT SPACE"
                          " TO MOVE THE CAMERA RELATIVE TO THE ORIENTATION  PRESS R TO RESET THE CAMERA  YOU CAN CHANGE"
                          " THE WAY YOU FACE USING YOUR MOUSE JUST DRAG IT, NO ROLL ONLY YAW AND PITCH"
                          "  FOR THIS TO WORK WE HAD TO HIDE THE MOUSE AND FIX IT. PRESS ALT TO TOGGLE THIS."
                          "  GET CLOSE TO THE RUBIKS CUBES AND PRESS LEFT MOUSE BUTTON  HOLD LEFT MOUSE BUTTON AND MOVE MOUSE TO ROTATE THE CUBE"
                          "  PRESS M TO SHUFFLE THE BIG CUBE  PRESS K TO SHUFFLE THE SMALL CUBE  PRESS N TO SOLVE THE BIG CUBE  PRESS J TO SOLVE THE SMALL CUBE";

    float messageSpeed = 10.0f;
};


#endif //HW2_H
