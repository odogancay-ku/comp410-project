//
// Created by ofarukdogancay on 4/25/24.
//

#ifndef COMP410_PROJECT_SEGMENTDISPLAY_H
#define COMP410_PROJECT_SEGMENTDISPLAY_H


#include "Object.h"

class SegmentDisplay{

    std::map<char *, Object *> segments;
    std::map<char *, glm::vec3> localPositions;
    std::map<char*, glm::quat> localOrientations;
    glm::vec3 position;
    glm::quat orientation;

    float scale;
    float segmentWidth = 0.1f;



public:

    SegmentDisplay(glm::vec3 position, glm::quat orientation, float scale);

    void lightSegment(char *segmentName);

    void unlightSegment(char *segmentName);

    void updateSegments();

    std::vector<Object*> getAllSegmentsAsVector();

    void displayCharacter(char c);

    void closeAllSegments();

};


#endif //COMP410_PROJECT_SEGMENTDISPLAY_H
