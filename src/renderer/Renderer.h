//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_RENDERER_H
#define HOMEWORK1_RENDERER_H


#include <glew.h>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include "../controller/WindowController.h"
#include "../objects/physics/Object.h"

class Renderer {

private:

    static inline Renderer *activeInstance = nullptr;
    static std::vector<Renderer> instances;

    GLuint VBO, VAO, EBO;
    GLuint instanceModelMatrixVBO;
    GLuint colorVertexVBO;
    GLfloat nearPlane = 0.1f;
    GLfloat farPlane = 500.0f;

public:

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    GLuint shaderProgram;


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

        std::cout << "Vertex shader source: " << vertexShaderSource << std::endl;
        std::cout << "Fragment shader source: " << fragmentShaderSource << std::endl;

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

        checkOpenGLError("loadShaderProgram");
        std::cout << "Shader program loaded " << _shaderProgram << std::endl;
        return _shaderProgram;
    }

    // Change shader program
    void useShaderProgram(GLuint _shaderProgram) {
        shaderProgram = _shaderProgram;
        glUseProgram(shaderProgram);

        checkOpenGLError("useShaderProgram");
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
        glGenBuffers(1, &instanceModelMatrixVBO);
        glGenBuffers(1, &colorVertexVBO);
        checkOpenGLError("initializeGL");
    }


    static void nextDrawMode();

    static void nextFocusedDrawMode();

    static void checkOpenGLError(const std::string &at);

    void drawInstancesOfModel(ModelTypes type, std::vector<Object*> *pVector, bool hitboxes = false);


    void createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight);

    void createAndSetViewMatrix();

    void setLight(glm::vec3 lightPos, glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular);

    void setMaterial(glm::vec3 materialAmbient, glm::vec3 materialDiffuse, glm::vec3 materialSpecular, float shininess);

    void setMaterial(Material material);

    static int drawMode;

    void static switchDrawMode();


};


#endif //HOMEWORK1_RENDERER_H
