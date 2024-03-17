//
// Created by ofaru on 16.03.2024.
//

#include <sstream>
#include <fstream>
#include "Renderer.h"
#include "../camera/Camera.h"

std::string Renderer::readShaderFile(const char *path) {

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}


void Renderer::nextDrawMode() {

}

void Renderer::nextFocusedDrawMode() {

}

void Renderer::checkOpenGLError(const std::string &at) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error at " << at << ": " << error << std::endl;
    }
}

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};


void Renderer::drawInstancesOfModel(ModelTypes type, std::vector<Object> *pVector) {
    // Set up VBOs for vertex data and instance-specific data
    ModelData modelData = ResourceManager::getModel(type);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices = modelData.indices;
    GLuint instanceCount = pVector->size();

    for (int i = 0; i < modelData.vertices.size(); ++i) {
        Vertex vertex{};
        vertex.position = modelData.vertices[i];
        vertex.normal = modelData.normals[i];
        vertices.push_back(vertex);
    }

    std::vector<glm::vec4> modelMatrixColumns = {};
    std::vector<glm::vec3> colors = {};

    for (auto &object : *pVector) {
        glm::mat4 modelMatrix = object.getModelMatrix();
        modelMatrixColumns.push_back(modelMatrix[0]);
        modelMatrixColumns.push_back(modelMatrix[1]);
        modelMatrixColumns.push_back(modelMatrix[2]);
        modelMatrixColumns.push_back(modelMatrix[3]);
    }

    for (auto colorVertex : modelData.colorVertices) {
        colors.push_back(colorVertex);
    }

    /**
    layout (location = 0) in vec4 ModelMatrix_Column0; // Same for all indices in same instance
    layout (location = 1) in vec4 ModelMatrix_Column1; // Same for all indices in same instances
    layout (location = 2) in vec4 ModelMatrix_Column2; // Same for all indices in same instances
    layout (location = 3) in vec4 ModelMatrix_Column3; // Same for all indices in same instances
    layout (location = 4) in vec4 localPosition; // Same for same indices in different instances
    layout (location = 5) in vec4 vertexNormal; // Same for same indices in different instances
    layout (location = 6) in vec4 vertexColor; // Different for all vertices

    uniform mat4 ProjectionMatrix; // Same for all instances and vertices
    uniform mat4 ViewMatrix; // Same for all instances and vertices
     */

    // Bind vertex buffers for position, normal, and color
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers for position and normal
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);


    // Bind buffer for instance-specific data (model matrix columns)
    glBindBuffer(GL_ARRAY_BUFFER, instanceModelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceCount * 4 * sizeof(glm::vec4), modelMatrixColumns.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers for instance-specific data
    // Assuming model matrix columns start from location 2
    for (GLuint i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(i, 1); // Set divisor for instanced rendering
    }


    glDrawArraysInstanced(GL_TRIANGLES, 0, indices.size(), instanceCount);

    std::cout << "Draw call using instanced rendering" << std::endl;

}


void Renderer::createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight) {
    float aspectRatio = (float) _windowWidth / (float) _windowHeight;

    float fov = Camera::getActiveInstance()->fov;

    if (aspectRatio < 1.0f) {
        fov = 2 * atan(tan(fov * (M_PI / 180) / 2) * (1 / aspectRatio)) * (180 / M_PI);
    }

    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

    // Pass the projection matrix to the shader program
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Renderer::createAndSetViewMatrix() {

    Camera camera = *Camera::getActiveInstance();

    // Calculate the new direction vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw + 90)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = -1 * sin(glm::radians(camera.yaw + 90)) * cos(glm::radians(camera.pitch));
    direction = normalize(direction);

    std::cout << "Direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;

    // Calculate the right and up vector
    glm::vec3 right = normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = normalize(cross(right, direction));


    // Create the view matrix
    viewMatrix = glm::lookAt(camera.position, camera.position + direction, up);

    // Pass the view matrix to the shader program
    GLint viewLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

glm::vec3 Renderer::simulateMVP(glm::vec3 originalPosition, glm::mat4 modelMatrix) {
    glm::vec4 vertex = glm::vec4(0.5, 0.5f, 0.5f, 1.0f);
    glm::vec4 position = glm::vec4(originalPosition, 1.0f);

    std::cout << "Original position: " << (originalPosition.x + vertex.x * modelMatrix[0][0]) << " "
              << (originalPosition.y + vertex.y * modelMatrix[1][1]) << " "
              << (originalPosition.z + vertex.z * modelMatrix[2][2]) << std::endl;

    std::cout << "Model matrix: " << modelMatrix[0][0] << " " << modelMatrix[1][0] << " " << modelMatrix[2][0] << " "
              << modelMatrix[3][0] << std::endl;
    std::cout << "Model matrix: " << modelMatrix[0][1] << " " << modelMatrix[1][1] << " " << modelMatrix[2][1] << " "
              << modelMatrix[3][1] << std::endl;
    std::cout << "Model matrix: " << modelMatrix[0][2] << " " << modelMatrix[1][2] << " " << modelMatrix[2][2] << " "
              << modelMatrix[3][2] << std::endl;
    std::cout << "Model matrix: " << modelMatrix[0][3] << " " << modelMatrix[1][3] << " " << modelMatrix[2][3] << " "
              << modelMatrix[3][3] << std::endl;

    std::cout << "View matrix: " << viewMatrix[0][0] << " " << viewMatrix[1][0] << " " << viewMatrix[2][0] << " "
              << viewMatrix[3][0] << std::endl;
    std::cout << "View matrix: " << viewMatrix[0][1] << " " << viewMatrix[1][1] << " " << viewMatrix[2][1] << " "
              << viewMatrix[3][1] << std::endl;
    std::cout << "View matrix: " << viewMatrix[0][2] << " " << viewMatrix[1][2] << " " << viewMatrix[2][2] << " "
              << viewMatrix[3][2] << std::endl;
    std::cout << "View matrix: " << viewMatrix[0][3] << " " << viewMatrix[1][3] << " " << viewMatrix[2][3] << " "
              << viewMatrix[3][3] << std::endl;

    std::cout << "Projection matrix: " << projectionMatrix[0][0] << " " << projectionMatrix[1][0] << " "
              << projectionMatrix[2][0] << " " << projectionMatrix[3][0] << std::endl;
    std::cout << "Projection matrix: " << projectionMatrix[0][1] << " " << projectionMatrix[1][1] << " "
              << projectionMatrix[2][1] << " " << projectionMatrix[3][1] << std::endl;
    std::cout << "Projection matrix: " << projectionMatrix[0][2] << " " << projectionMatrix[1][2] << " "
              << projectionMatrix[2][2] << " " << projectionMatrix[3][2] << std::endl;
    std::cout << "Projection matrix: " << projectionMatrix[0][3] << " " << projectionMatrix[1][3] << " "
              << projectionMatrix[2][3] << " " << projectionMatrix[3][3] << std::endl;

    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    glm::vec4 mvpPosition = mvp * vertex;
    mvpPosition /= mvpPosition.w;

    std::cout << "Position: " << mvpPosition.x << " " << mvpPosition.y << " " << mvpPosition.z << " " << mvpPosition.w
              << std::endl;

    return position;
}