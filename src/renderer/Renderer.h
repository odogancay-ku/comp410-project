//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_RENDERER_H
#define HOMEWORK1_RENDERER_H


#include <glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include "../controller/WindowController.h"
#include "../objects/physics/Object.h"

class Renderer {

private:

    static Renderer *activeInstance;
    static std::vector<Renderer> instances;

    GLuint shaderProgram;
    GLuint VBO, VAO, EBO;

public:
    static Renderer *getActiveInstance() {
        if (activeInstance == nullptr) {
            activeInstance = new Renderer();
        }
        return activeInstance;
    };

    static std::string readShaderFile(const char *path);

    // Load shader program
    GLuint loadShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath) {
        // Load vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vertexShaderSource = readShaderFile(vertexShaderPath).c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        delete[] vertexShaderSource;

        // Load fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentShaderSource = readShaderFile(fragmentShaderPath).c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        delete[] fragmentShaderSource;

        // Create shader program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }

    // Change shader program
    void useShaderProgram(GLuint shaderProgram) {
        glUseProgram(shaderProgram);
    }

    // Set view matrix uniform value in shader program
    void setViewMatrix(GLuint shaderProgram, const glm::mat4 &viewMatrix) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
    }

    // Set projection matrix uniform value in shader program
    void setProjectionMatrix(GLuint shaderProgram, const glm::mat4 &projectionMatrix) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projectionMatrix[0][0]);
    }

    // Generate the projection matrix using the existing values
    glm::mat4 calculateProjectionMatrix(GLfloat _windowWidth, GLfloat _windowHeight, GLfloat fov, GLfloat nearPlane,
                                GLfloat farPlane) {

        GLfloat windowWidth = _windowWidth;
        GLfloat windowHeight = _windowHeight;
        GLfloat aspectRatio = windowWidth / windowHeight;

        if (aspectRatio < 1.0f) {
            fov = 2 * atan(tan(fov * (M_PI / 180) / 2) * (1 / aspectRatio)) * (180 / M_PI);
        }


        // Calculate the projection matrix
        float f = 1.0f / tan(fov * 0.5f * (M_PI / 180.0f));
        float rangeInv = 1.0f / (nearPlane - farPlane);

        glm::mat4 projectionMatrix = {
                f / aspectRatio, 0.0f,0.0f,                                     0.0f,
                0.0f,            f,    0.0f,                                    0.0f,
                0.0f,            0.0f, (nearPlane + farPlane) * rangeInv,       -1.0f,
                0.0f,            0.0f, nearPlane * farPlane * rangeInv * 2.0f, 0.0f};

        return projectionMatrix;
    }


    void initializeGL() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void drawObject(Object &object);

    void addObjectVerticesAndIndices(Object &object, std::vector<GLfloat> data, std::vector<GLuint> indices);

    static void nextDrawMode();

    static void nextFocusedDrawMode();
};


#endif //HOMEWORK1_RENDERER_H
