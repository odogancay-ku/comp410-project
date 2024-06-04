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
};

struct Model {
    std::vector<Mesh> meshes;
};

#endif //MESH_H
