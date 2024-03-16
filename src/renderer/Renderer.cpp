//
// Created by ofaru on 16.03.2024.
//

#include <sstream>
#include <fstream>
#include "Renderer.h"

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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr); // Use glDrawElements instead of glDrawArrays

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

void Renderer::checkOpenGLError(const std::string& at) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error at "<< at <<": "<< error << std::endl;
    }
}

void Renderer::drawInstancesOfModel(const ModelTypes types, std::vector<Object> *pVector) {

    // Set up instance data (e.g., model matrices)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numInstances, instanceData, GL_STATIC_DRAW);

    for (unsigned int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
        glVertexAttribDivisor(3 + i, 1); // This attribute advances once per instance
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, numVertices, numInstances);

}

