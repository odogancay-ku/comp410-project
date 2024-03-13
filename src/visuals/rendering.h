//
// Created by ofaru on 12.03.2024.
//

#ifndef RENDERING_H
#define RENDERING_H

#include "Angel.h"
#include "../physics/object.h"
#include "camera.h"


class Renderer {
public:

    Renderer();

    Renderer(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);

    std::basic_string<char> vertexShaderSource;
    std::basic_string<char> fragmentShaderSource;


    GLuint loadShaders(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);


    void
    drawTriangle(const vec3 &vertex1, const vec3 &vertex2, const vec3 &vertex3, const vec3 &normal1,
                 const vec3 &normal2,
                 const vec3 &normal3, const vec3 &color1, const vec3 &color2, const vec3 &color3);


    void drawObject(const Object &object);

    void drawObjects(const std::vector<Object>& objects);


    GLuint programID;
    float aspectRatio;
    float boundingBoxWidth = 10.0f;
    float farPlane = 500.0f;
    float nearPlane = 0.1f;
    int windowWidth;
    int windowHeight;

    void setCamera(const vec3 &position, float yaw, float pitch);

    void createAndSetPerspectiveProjectionMatrix(int windowWidth, int windowHeight, float fov);
};

#endif //RENDERING_H
