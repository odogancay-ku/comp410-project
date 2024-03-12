//
// Created by ofaru on 12.03.2024.
//

#include <fstream>
#include <sstream>
#include <vector>
#include "rendering.h"
#include "utility.h"

void Renderer::drawObjects(const std::vector<Object>& objects) {
    for (const auto& object : objects) {
        drawObject(object);
    }
}

void Renderer::drawObject(const Object &object) {

    glUseProgram(programID);
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

        drawTriangle(object.position + vertex1, object.position + vertex2, object.position + vertex3,
                     normal1, normal2, normal3,
                     color1, color2, color3);
    }
}

void Renderer::loadShaders(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
    vertexShaderSource = readShaderSource(vertexShaderFilename).c_str();
    fragmentShaderSource = readShaderSource(fragmentShaderFilename).c_str();
}

void Renderer::compileAndLinkShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::drawTriangle(const vec3 &vertex1, const vec3 &vertex2, const vec3 &vertex3,
                            const vec3 &normal1, const vec3 &normal2, const vec3 &normal3,
                            const vec3 &color1, const vec3 &color2, const vec3 &color3) {
    GLfloat vertices[] = {
        vertex1.x, vertex1.y, vertex1.z, color1.x, color1.y, color1.z,
        vertex2.x, vertex2.y, vertex2.z, color2.x, color2.y, color2.z,
        vertex3.x, vertex3.y, vertex3.z, color3.x, color3.y, color3.z
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO

    // Draw the triangle
    glUseProgram(programID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Renderer::Renderer(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)  {
    loadShaders(vertexShaderFile, fragmentShaderFile);
    compileAndLinkShaders();
}

void Renderer::createAndSetPerspectiveProjectionMatrix(int windowWidth, int windowHeight)
{
    // Define projection matrix parameters
    aspectRatio = (float)windowWidth / (float)windowHeight;

    boundingBoxWidth = nearPlane * tan(fov * 0.5f * (M_PI / 180.0f)) * 2.0f;
    boundingBoxHeight = boundingBoxWidth / aspectRatio;

    boundingBoxLeft = -boundingBoxWidth / 2;
    boundingBoxRight = boundingBoxWidth / 2;
    boundingBoxBottom = -boundingBoxHeight / 2;
    boundingBoxTop = boundingBoxHeight / 2;
    boundingBoxBack = -nearPlane - boundingBoxWidth;
    boundingBoxFront = -nearPlane;

    // Print aspect ratio
    std::cout << "Aspect ratio: " << aspectRatio << std::endl;

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


std::string readShaderSource(const std::string &filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    // Print the source
    std::cout << ss.str() << std::endl;
    return ss.str();
}