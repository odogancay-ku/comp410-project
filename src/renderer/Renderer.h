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

    static inline Renderer *activeInstance = nullptr;
    static std::vector<Renderer> instances;

    GLuint shaderProgram;
    GLuint VBO, VAO, EBO;

public:
    static Renderer *getActiveInstance() {
        if (activeInstance == nullptr) {
            exit(1);
        }
        return activeInstance;
    };

    static void setActiveInstance(Renderer *_activeInstance) {
        Renderer::activeInstance = _activeInstance;
    }

    static std::string readShaderFile(const char *path);

    // Load shader program
    GLuint loadShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath) {

        std::cout << "Loading shader program" << std::endl;
        std::basic_string<char> vertexShaderSource = readShaderFile(vertexShaderPath);
        std::basic_string<char> fragmentShaderSource = readShaderFile(fragmentShaderPath);

        // Create a vertex shader object
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Attach the vertex shader source code to the shader object
        const GLchar *vertexShaderSourceCStr = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, nullptr);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        // Check if the vertex shader compiled successfully
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar *fragmentShaderSourceCStr = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        // Create a shader program object
        GLuint _shaderProgram = glCreateProgram();

        // Attach the vertex and fragment shaders to the shader program
        glAttachShader(_shaderProgram, vertexShader);
        glAttachShader(_shaderProgram, fragmentShader);

        // Link the shaders together into the shader program
        glLinkProgram(_shaderProgram);

        // Check if the shaders were linked successfully
        glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            // The shaders were not linked successfully
            char infoLog[512];
            glGetProgramInfoLog(_shaderProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }

        return _shaderProgram;
    }

    // Change shader program
    void useShaderProgram(GLuint _shaderProgram) {
        shaderProgram = _shaderProgram;
        glUseProgram(shaderProgram);

        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }
    }

    // Set view matrix uniform value in shader program
    void setViewMatrix(const glm::mat4 &viewMatrix) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ViewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
        checkOpenGLError("setViewMatrix");
    }

    // Set projection matrix uniform value in shader program
    void setProjectionMatrix(const glm::mat4 &projectionMatrix) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
        checkOpenGLError("setProjectionMatrix");
    }


    void setModelMatrix(glm::mat4& modelMatrix) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ModelMatrix"), 1, GL_FALSE, &modelMatrix[0][0])
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

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
    }

    void drawObject(Object &object);

    void addObjectVerticesAndIndices(Object &object, std::vector<GLfloat> &data, std::vector<GLuint> &indices);

    static void nextDrawMode();

    static void nextFocusedDrawMode();

    static void checkOpenGLError(const std::string &at);

    void drawInstancesOfModel(const ModelTypes types, std::vector<Object> *pVector);
};


#endif //HOMEWORK1_RENDERER_H
