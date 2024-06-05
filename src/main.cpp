#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "controller/WindowController.h"
#include "camera/Camera.h"
#include "controller/InputController.h"
#include "game/Game.h"
#include "objects/graphics/modelLoader.h"
#include "objects/Entity.h"
#include "objects/physics/Collider.h"

// Main function
int main() {


    glfwWindowHint(GLFW_SAMPLES, 4);
    WindowController::createFullscreenWindow("Homework 1");
    Renderer::initializeGL();

    Renderer::objectShader = new Shader("shaders/pbr_vertex.glsl", "shaders/pbr_fragment.glsl");
//    Renderer::objectShader = new Shader("shaders/blinn_phong_vertex.glsl", "shaders/blinn_phong_fragment.glsl");
    Renderer::objectShader->use();

//    auto* model = new Model(loadModel("assets/models/wolf/Wolf.fbx"));
//    auto* model = new Model(loadModel("assets/models/cerberus.glb"));
    auto* model = new Model(loadModel("assets/models/saturn/asteroid.glb"));
//    auto* model = new Model(loadModel("assets/models/dogancay.off"));
    std::cout << "Model loaded" << std::endl;

    auto *camera = new Camera();
    Camera::setActiveInstance(camera);
    Renderer::createAndSetPerspectiveProjectionMatrix(WindowController::width, WindowController::height);

    Game *game = Game::getInstance();

    InputController::init(WindowController::window);

    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();

    double dt_step = 0.008f;

    std::cout << glm::to_string(Camera::getActiveInstance()->getViewMatrix()) << std::endl;
    std::cout << glm::to_string(Renderer::projectionMatrix) << std::endl;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Rotate x -90 and y +90
    modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, 0.0f, 0.0f));
//    modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Set camera to look at the model

    Light light;
    light.lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
    light.lightAmbient = glm::vec3(0.2f);
    light.lightDiffuse = glm::vec3(1.0f);
    light.lightSpecular = glm::vec3(0.3f);

    Renderer::setLight(&light);

    InputController::init(WindowController::window);


    Scene scene;

    Entity* entity = scene.createEntity();
    entity->hitbox = Hitbox::newCubeCollider();
    entity->model = static_cast<const std::shared_ptr<Model>>(model);
    entity->position = glm::vec3(10.0f, 0.0f, 0.0f);
    entity->model->instanceId = 1;

    while (!glfwWindowShouldClose(WindowController::window)) {
        // Clear the screen

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        scene.update(deltaTime);

        scene.draw(*Renderer::objectShader);


        checkOpenGLError("main loop");


        glfwSwapBuffers(WindowController::window);
        glfwPollEvents();

        frameCount++;
        double currentFPSTime = glfwGetTime();
        double elapsedTime = currentFPSTime - lastFPSTime;
        if (elapsedTime >= 5.0) {
            std::cout << "FPS: " << frameCount / elapsedTime << std::endl;
            frameCount = 0;
            lastFPSTime = currentFPSTime;
        }


    }

    // Terminate GLFW
    glfwTerminate();
}