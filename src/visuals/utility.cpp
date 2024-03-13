//
// Created by ofaru on 12.03.2024.
//

#include "utility.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string readShaderSource(const std::string &filename)
{

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

GLuint loadShaders()
{
    // Load and compile the shaders

    std::string vertexShaderSource = readShaderSource("./shaders/vertexShader.glsl");
    std::string fragmentShaderSource = readShaderSource("./shaders/fragmentShader.glsl");

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
    if (!success)
    {
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
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create a shader program object
    GLuint shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the shaders together into the shader program
    glLinkProgram(shaderProgram);

    // Check if the shaders were linked successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        // The shaders were not linked successfully
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    return shaderProgram;
}