//
// Created by ofarukdogancay on 6/5/24.
//

#ifndef COMP410_PROJECT_MODEL_H
#define COMP410_PROJECT_MODEL_H

#include <glm/glm.hpp>
#include "string"
#include "vector"
#include "Mesh.h"
#include "renderer/Shader.h"
#include <map>

class Model {
public:
    int instanceId = 0;
    std::vector<Mesh> meshes;
    void draw(Shader& shader, glm::mat4 transform);

    void drawInstanced(Shader &shader, const std::vector<glm::mat4>& transforms);
};


#endif //COMP410_PROJECT_MODEL_H
