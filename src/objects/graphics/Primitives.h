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
    DIFFUSE,
    SPECULAR,
    METALLIC,
    EMISSIVE,
    NORMAL,
    HEIGHT,
    AMBIENT_OCCLUSION,
    REFLECTION,
    LIGHT_MAP,
    OPACITY
};

struct Texture {
    unsigned int id;
    TextureType type;
};



#endif PRIMITIVES_H
