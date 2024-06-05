#include <sstream>
#include <fstream>
#include "Renderer.h"
#include "camera/Camera.h"

int Renderer::drawMode = 0;



void Renderer::nextDrawMode() {

    // Set the draw mode to the next mode
    drawMode = (drawMode + 1) % 3;

    switchDrawMode();

}

void Renderer::switchDrawMode() {
    switch (drawMode) {
        case 0:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 1:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 2:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }

}


void checkOpenGLError(const std::string &at) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error at " << at << ": " << error << " " << gluErrorString(error) << std::endl;
    }
}


void Renderer::createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight) {
    float aspectRatio = (float) _windowWidth / (float) _windowHeight;

    float fov = Camera::getActiveInstance()->fov;

    if (aspectRatio < 1.0f) {
        fov = 2 * atan(tan(fov * (M_PI / 180) / 2) * (1 / aspectRatio)) * (180 / M_PI);
    }

    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

    // Pass the projection matrix to the shader program
    objectShader->setMat4("projection", projectionMatrix);
}


void Renderer::setLight(Light* light) {

    objectShader->setVec3("light.position", light->lightPos);
    objectShader->setVec3("light.ambient", light->lightAmbient);
    objectShader->setVec3("light.diffuse", light->lightDiffuse);
    objectShader->setVec3("light.specular", light->lightSpecular);
    objectShader->setFloat("light.constant", light->constant);
    objectShader->setFloat("light.linear", light->linear);
    objectShader->setFloat("light.quadratic", light->quadratic);

}

void Renderer::setMaterial(glm::vec3 materialAmbient, glm::vec3 materialDiffuse, glm::vec3 materialSpecular,
                           float shininess, Shader* shader) {

    shader->setVec3("material.ambient", materialAmbient);
    shader->setVec3("material.diffuse", materialDiffuse);
    shader->setVec3("material.specular", materialSpecular);
    shader->setFloat("material.shininess", shininess);

}



