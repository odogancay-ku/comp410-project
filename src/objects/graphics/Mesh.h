#ifndef MESH_H
#define MESH_H


#include "Primitives.h"

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif //MESH_H
