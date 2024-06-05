//
// Created by ofarukdogancay on 5/25/24.
//

#ifndef LIGHT_H
#define LIGHT_H



class Light {
public:
    glm::vec3 lightPos;
    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

};

#endif //LIGHT_H
