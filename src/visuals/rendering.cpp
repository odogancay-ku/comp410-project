//
// Created by ofaru on 12.03.2024.
//
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rendering.h"
#include "utility.h"


void Renderer::drawObjects(const std::vector<Object> &objects) {
    for (const auto &object: objects) {
        drawObject(object);
    }

}

void Renderer::drawObject(const Object &object) {

    GLint translationLoc = glGetUniformLocation(programID, "translation");
    glUniform3f(translationLoc, object.position.x, object.position.y, object.position.z);

    for (int i = 0; i < object.indices.size(); i += 3) {


        vec3 vertex1 = object.vertices[object.indices[i]];
        vec3 vertex2 = object.vertices[object.indices[i + 1]];
        vec3 vertex3 = object.vertices[object.indices[i + 2]];

        vec3 normal1 = object.normals[object.indices[i]];
        vec3 normal2 = object.normals[object.indices[i + 1]];
        vec3 normal3 = object.normals[object.indices[i + 2]];

        vec3 color1 = object.colors[object.indices[i]];
        vec3 color2 = object.colors[object.indices[i + 1]];
        vec3 color3 = object.colors[object.indices[i + 2]];

        drawTriangle(vertex1, vertex2, vertex3,
                     normal1, normal2, normal3,
                     color1, color2, color3);
    }
}


void Renderer::drawTriangle(const vec3 &vertex1, const vec3 &vertex2, const vec3 &vertex3,
                            const vec3 &normal1, const vec3 &normal2, const vec3 &normal3,
                            const vec3 &color1, const vec3 &color2, const vec3 &color3) {
    GLfloat vertices[] = {
            vertex1.x, vertex1.y, vertex1.z, normal1.x, normal1.y, normal1.z,
            vertex2.x, vertex2.y, vertex2.z, normal2.x, normal2.y, normal2.z,
            vertex3.x, vertex3.y, vertex3.z, normal3.x, normal3.y, normal3.z
    };


    GLuint VBO, VAO;


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLint colorLoc = glGetUniformLocation(programID, "objectColor");

    // Set the value of the uniform variable
    glUniform3f(colorLoc, color1.x, color1.y, color1.z);

    glDrawArrays(GL_TRIANGLES, 0, 3);

}

GLuint Renderer::loadShaders(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
    vertexShaderSource = readShaderSource(vertexShaderFilename);
    fragmentShaderSource = readShaderSource(fragmentShaderFilename);

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
    GLuint shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the shaders together into the shader program
    glLinkProgram(shaderProgram);

    // Check if the shaders were linked successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        // The shaders were not linked successfully
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    programID = shaderProgram;

    glUseProgram(programID);

    return shaderProgram;

}


Renderer::Renderer(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {
    loadShaders(vertexShaderFile, fragmentShaderFile);
}

void Renderer::createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight, float fov) {
    // Define projection matrix parameters
    std::cout << "Creating perspective projection matrix" << std::endl;
    std::cout << "Window width: " << _windowWidth << std::endl;
    std::cout << "Window height: " << _windowHeight << std::endl;
    windowWidth = _windowWidth;
    windowHeight = _windowHeight;
    aspectRatio = (float) windowWidth / (float) windowHeight;


    // Calculate the projection matrix
    float f = 1.0f / tan(fov * 0.5f * (M_PI / 180.0f));
    float rangeInv = 1.0f / (nearPlane - farPlane);

    float projectionMatrix[16] = {
            f / aspectRatio, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (nearPlane + farPlane) * rangeInv, -1.0f,
            0.0f, 0.0f, nearPlane * farPlane * rangeInv * 2.0f, 0.0f};


    // Pass the projection matrix to the shader program
    GLint projectionLoc = glGetUniformLocation(programID, "ProjectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);
}

void Renderer::setCamera(const vec3 &position, float yaw, float pitch) {
    // Calculate the new direction vector
    vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = normalize(direction);

    // Calculate the right and up vector
    vec3 right = normalize(cross(direction, vec3(0.0f, 1.0f, 0.0f)));
    vec3 up = normalize(cross(right, direction));

    glm::vec3 glmPosition = glm::vec3(position.x, position.y, position.z);
    glm::vec3 glmDirection = glm::vec3(direction.x, direction.y, direction.z);
    glm::vec3 glmUp = glm::vec3(up.x, up.y, up.z);

    // Create the view matrix
    glm::mat4 glmView = glm::lookAt(glmPosition, glmPosition + glmDirection, glmUp);

    // Pass the view matrix to the shader program
    GLint viewLoc = glGetUniformLocation(programID, "ViewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glmView));


}

Renderer::Renderer() {
    std::cout << "Renderer created" << std::endl;
}

