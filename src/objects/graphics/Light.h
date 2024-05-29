//
// Created by ofarukdogancay on 5/25/24.
//

#ifndef COMP410_PROJECT_LIGHT_H
#define COMP410_PROJECT_LIGHT_H


struct Light {
    glm::vec3 lightPos;
    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

} typedef;

#endif //COMP410_PROJECT_LIGHT_H
