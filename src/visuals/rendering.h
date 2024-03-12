//
// Created by ofaru on 12.03.2024.
//

#ifndef RENDERING_H
#define RENDERING_H

#include "Angel.h"
#include "../physics/object.h"


class Renderer {
public:

    Renderer(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);

    const char* vertexShaderSource;
    const char* fragmentShaderSource;


    void loadShaders(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

    void compileAndLinkShaders();

    void
    drawTriangle(const vec3 &vertex1, const vec3 &vertex2, const vec3 &vertex3, const vec3 &normal1,
                 const vec3 &normal2,
                 const vec3 &normal3, const vec3 &color1, const vec3 &color2, const vec3 &color3);


    void drawObject(const Object &object);

    void drawObjects(const std::vector<Object>& objects);

    void createAndSetPerspectiveProjectionMatrix(int windowWidth, int windowHeight);

private:
    GLuint programID;
    float aspectRatio;
    float boundingBoxWidth;
    float boundingBoxHeight;
    float boundingBoxLeft;
    float boundingBoxRight;
    float boundingBoxBottom;
    float boundingBoxTop;
    float boundingBoxBack;
    float boundingBoxFront;
    float farPlane = 100.0f;
    float nearPlane = 9.9f;
    float fov = 45.0f;

};

#endif //RENDERING_H
