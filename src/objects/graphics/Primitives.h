#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include "string"
#include "vector"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

enum TextureType {
    ALBEDO,
    ROUGHNESS,
    NORMAL,
    METALLIC,
    AMBIENT_OCCLUSION,
};

struct Texture {
    unsigned int id;
    std::string path;
    TextureType type;
};


#endif PRIMITIVES_H
