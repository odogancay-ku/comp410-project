//
// Created by ofarukdogancay on 6/5/24.
//

#ifndef COMP410_PROJECT_MESH_H
#define COMP410_PROJECT_MESH_H

#include <glm/glm.hpp>
#include "string"
#include "vector"
#include "Primitives.h"
#include "renderer/Shader.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture roughness;
    Texture ambientOcclusion;
    void draw(Shader& shader, glm::mat4 transform) const;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint instanceVBO = 0;

    void setupMesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const std::vector<Texture>& textures);
    void setupInstancedBuffer(std::vector<glm::mat4> &transforms) ;
    void drawInstanced(Shader &shader, std::vector<glm::mat4> &transforms) ;

};

#endif //COMP410_PROJECT_MESH_H
