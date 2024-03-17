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

void Renderer::drawObject(Object &object) {

    std::vector<GLfloat> data;
    std::vector<GLuint> indices;

    addObjectVerticesAndIndices(object, data, indices);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    checkOpenGLError("BIND VBO");
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data[0], GL_STATIC_DRAW);
    checkOpenGLError("BUFFER DATA");

    glBindVertexArray(VAO);
    checkOpenGLError("BIND VAO");

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // World position translation attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *) (9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0],
                 GL_STATIC_DRAW); // Upload the indices to the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,
                   nullptr); // Use glDrawElements instead of glDrawArrays

}

void Renderer::addObjectVerticesAndIndices(Object &object, std::vector<GLfloat> &data, std::vector<GLuint> &indices) {

    if (object.isHidden) {
        return;
    }

    ModelData modelData = ResourceManager::getModel(object.modelType);

    for (int i = 0; i < modelData.vertices.size(); ++i) {

        glm::vec3 vertex = modelData.vertices[i];
        glm::vec3 normal = modelData.normals[i];
        glm::vec3 color = modelData.colorVertices[i];

        data.insert(data.end(), {vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, color.x, color.y, color.z,
                                 object.position.x, object.position.y, object.position.z});
    }

    for (GLuint index: modelData.indices) {
        indices.push_back(index);
    }


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

struct InstanceData {
    glm::mat4 modelMatrix;
    std::vector<glm::vec3> colorVertices; // Instance-specific color vertices
};

void Renderer::drawInstancesOfModel(const ModelTypes type, std::vector<Object> *pVector) {
    // Set up VBOs for vertex data and instance-specific data
    ModelData modelData = ResourceManager::getModel(type);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices = modelData.indices;

    for (int i = 0; i < modelData.vertices.size(); ++i) {
        Vertex vertex{};
        vertex.position = modelData.vertices[i];
        vertex.normal = modelData.normals[i];
        vertices.push_back(vertex);
    }

    std::vector<InstanceData> instanceDatas{};
    for (auto &object: *pVector) {
        InstanceData instanceData{};
        instanceData.modelMatrix = object.getModelMatrix();

        glm::mat4 p = calculateProjectionMatrix(1920, 1000, 90, 0.1f, 500.0f);
        glm::mat4 v;
        Camera::getActiveInstance()->getViewMatrix(v);
        glm::mat4 m = instanceData.modelMatrix;

        glm::mat4 mvp = p * v * m;

        glm::vec4 result = mvp * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        std::cout << "MVP: " << result.x << " " << result.y << " " << result.z << " " << result.w << std::endl;
        std::cout << object.position.x << " " << object.position.y << " " << object.position.z << std::endl;

        instanceData.colorVertices = modelData.colorVertices;
        instanceDatas.push_back(instanceData);
    }

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    checkOpenGLError("vertexBuffer");

    // Bind and fill instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, vboInstance);
    glBufferData(GL_ARRAY_BUFFER, instanceDatas.size() * sizeof(InstanceData), &instanceDatas[0], GL_STATIC_DRAW);
    checkOpenGLError("instanceBuffer");

    // Set up instance buffer object (VBO) for instance-specific color vertices
    size_t expectedSize = instanceDatas.size() * modelData.colorVertices.size() * sizeof(glm::vec3);
    glBindBuffer(GL_ARRAY_BUFFER, vboColorVertex);
    glBufferData(GL_ARRAY_BUFFER, expectedSize, nullptr, GL_STATIC_DRAW);
    checkOpenGLError("instanceColorBuffer");

    size_t offset = 0;
    for (const auto &instanceData: instanceDatas) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, instanceData.colorVertices.size() * sizeof(glm::vec3),
                        &instanceData.colorVertices[0]);
        offset += instanceData.colorVertices.size() * sizeof(glm::vec3);
    }
    checkOpenGLError("instanceColorBuffer");

    // Bind vertex array object (VAO)
    glBindVertexArray(VAO);

    // Vertex attribute setup
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
    // Vertex position attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
    glEnableVertexAttribArray(1); // Location 1: Position
    checkOpenGLError("vertexAttribute");

    // Vertex normal attribute (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(2); // Location 2: Normal
    checkOpenGLError("vertexAttribute");

    // ModelMatrix attribute (location = 0)
    // For model matrix, set up attributes for each column (4 attributes)
    for (unsigned int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(0 + i);
        glVertexAttribPointer(0 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) (sizeof(float) * i * 4));
        glVertexAttribDivisor(0 + i, 1); // This attribute advances once per instance
    }
    checkOpenGLError("modelMatrixAttribute");

    // Color attribute (location = 3)
    glBindBuffer(GL_ARRAY_BUFFER, vboColorVertex);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(3); // Location 3: Color
    glVertexAttribDivisor(3, 1); // This attribute advances once per instance
    checkOpenGLError("colorAttribute");

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    checkOpenGLError("indexBuffer");

    // Draw call
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    checkOpenGLError("drawInstancesOfModel");
}

void Renderer::createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight, float fov) {
    // Define projection matrix parameters
    float aspectRatio = (float) _windowWidth / (float) _windowHeight;

    if (aspectRatio < 1.0f) {
        fov = 2 * atan(tan(fov * (M_PI / 180) / 2) * (1 / aspectRatio)) * (180 / M_PI);
    }

    float nearPlane = 0.1f;
    float farPlane = 500.0f;

    // Calculate the projection matrix
    float f = 1.0f / tan(fov * 0.5f * (M_PI / 180.0f));
    float rangeInv = 1.0f / (nearPlane - farPlane);

    float projectionMatrix[16] = {
            f / aspectRatio, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (nearPlane + farPlane) * rangeInv, -1.0f,
            0.0f, 0.0f, nearPlane * farPlane * rangeInv * 2.0f, 0.0f};


    // Pass the projection matrix to the shader program
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);
}

void Renderer::createAndSetViewMatrix() {
    Camera::getActiveInstance()->setViewMatrix(shaderProgram);
}