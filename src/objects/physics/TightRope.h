//
// Created by ofarukdogancay on 4/25/24.
//

#ifndef COMP410_PROJECT_TIGHTROPE_H
#define COMP410_PROJECT_TIGHTROPE_H


#include "Object.h"

class TightRope : Object {

public:
    TightRope(glm::vec3 point1, glm::vec3 point2, float radius, int segments, glm::vec3 colorStart, glm::vec3 colorEnd);

    glm::vec3 start;
    glm::vec3 end;
    float radius;
    int segments;
    glm::vec3 colorStart;
    glm::vec3 colorEnd;

    void updateEnds(glm::vec3 point1, glm::vec3 point2);

};


#endif //COMP410_PROJECT_TIGHTROPE_H
