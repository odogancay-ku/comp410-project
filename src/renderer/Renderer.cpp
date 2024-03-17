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
    ModelData modelData = *ResourceManager::getModel(type);

    GLuint instanceCount = pVector->size();


    std::vector<glm::vec4> modelMatrixColumns = {};
    std::vector<glm::vec3> colors = {};

    for (auto &object: *pVector) {
        glm::mat4 modelMatrix = object.getModelMatrix();
        modelMatrixColumns.push_back(modelMatrix[0]);
        modelMatrixColumns.push_back(modelMatrix[1]);
        modelMatrixColumns.push_back(modelMatrix[2]);
        modelMatrixColumns.push_back(modelMatrix[3]);
    }

    colors.reserve(modelData.colorVertices.size());
    for (auto colorVertex: modelData.colorVertices) {
        colors.push_back(colorVertex);
    }

    /**
    layout (location = 0) in vec4 ModelMatrix_Column0; // Same for all indices in same instance
    layout (location = 1) in vec4 ModelMatrix_Column1; // Same for all indices in same instances
    layout (location = 2) in vec4 ModelMatrix_Column2; // Same for all indices in same instances
    layout (location = 3) in vec4 ModelMatrix_Column3; // Same for all indices in same instances
    layout (location = 4) in vec4 localPosition; // Same for same indices in different instances // Buffer already existing
    layout (location = 5) in vec4 vertexNormal; // Same for same indices in different instances // Buffer already existing
    layout (location = 6) in vec4 vertexColor; // Different for all vertices

    uniform mat4 ProjectionMatrix; // Same for all instances and vertices
    uniform mat4 ViewMatrix; // Same for all instances and vertices
     */

    // TODO USE LOADED DATA
    //std::cout << "Binding VAO: " << modelData.VAO << " " << modelData.VBO << " "<< modelData.EBO << std::endl;
    // Bind the VAO that you want to draw
    //glBindVertexArray(modelData.VAO);

    std::vector<glm::vec3> verticesAndNormals = {};

    for (int i = 0; i < modelData.vertices.size(); ++i) {
        verticesAndNormals.push_back(modelData.vertices[i]);
        // If normal is not available, use the vertex as normal
        if (i < modelData.normals.size()) {
            verticesAndNormals.push_back(modelData.normals[i]);
        } else {
            verticesAndNormals.push_back(modelData.vertices[i]);
        }
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesAndNormals.size() * sizeof(glm::vec3), &verticesAndNormals[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelData.indices.size() * sizeof(GLuint), &modelData.indices[0],
                 GL_STATIC_DRAW);


    // Set the vertex attribute pointers

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Bind this VBO and upload the data to it
    glBindBuffer(GL_ARRAY_BUFFER, instanceModelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrixColumns.size() * sizeof(glm::vec4), &modelMatrixColumns[0],
                 GL_STATIC_DRAW);



    // Set the vertex attribute pointers for the instance-specific data
    for (int i = 2; i < 6; i++) {

        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(GLfloat),  (GLvoid *) ((i-2) * 4 *sizeof(GLfloat)));
        glVertexAttribDivisor(i, 1);
        glEnableVertexAttribArray(i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, colorVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointer for the color data
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(6);



    glDrawElementsInstanced(GL_TRIANGLES, modelData.indices.size(), GL_UNSIGNED_INT, nullptr, instanceCount);

    glBindVertexArray(0);


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

    Camera* camera = Camera::getActiveInstance();

    if (camera->followObject != nullptr) {
        // Make sure up and direction is not the same
        glm::vec3 direction;
        direction = normalize(camera->followObject->position-camera->position);
        glm::vec3 absoluteUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = normalize(cross(direction, absoluteUp));
        glm::vec3 up = normalize(cross(right, direction));
        viewMatrix = glm::lookAt(camera->position, camera->followObject->position, up);

        // Update yaw and pitch too

        camera->yaw = glm::degrees(atan2(direction.z, direction.x));
        camera->pitch = glm::degrees(asin(direction.y));

    } else {
        // Calculate the new direction vector
        glm::vec3 direction;
        direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        direction.y = sin(glm::radians(camera->pitch));
        direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        direction = normalize(direction);


        // Calculate the right and up vector
        glm::vec3 right = normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = normalize(cross(right, direction));

        // Create the view matrix
        viewMatrix = glm::lookAt(camera->position, camera->position + direction, up);
    }



    // Pass the view matrix to the shader program
    GLint viewLoc = glGetUniformLocation(shaderProgram, "ViewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}