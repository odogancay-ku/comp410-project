#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include "string"
#include "vector"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

enum TextureType {
    ALBEDO,
    ROUGHNESS,
    DIFFUSE,
    SPECULAR,
    METALLIC,
    EMISSIVE,
    NORMAL,
    HEIGHT,
    AMBIENT_OCCLUSION,
    REFLECTION,
    LIGHT_MAP,
    OPACITY,
    UNKNOWN
};

struct Texture {
    unsigned int id;
    std::string path;
    TextureType type;
};



#endif PRIMITIVES_H
