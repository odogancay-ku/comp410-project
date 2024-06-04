#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "controller/WindowController.h"
#include "camera/Camera.h"
#include "controller/InputController.h"
#include "game/Game.h"
#include "objects/graphics/modelLoader.h"

// Main function
int main() {

    glfwWindowHint(GLFW_SAMPLES, 4);
    WindowController::createFullscreenWindow("Homework 1");




    Renderer::initializeGL();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Renderer::objectShader = new Shader("shaders/blinnPhongVertex.glsl", "shaders/blinnPhongFragment.glsl");
    Renderer::objectShader->use();

    std::cout << "Object Shader ID: " << Renderer::objectShader->getID() << std::endl;

    auto *camera = new Camera();
    Camera::setActiveInstance(camera);
    Renderer::createAndSetPerspectiveProjectionMatrix(WindowController::width, WindowController::height);

    ResourceManager::generateBuiltinModels();
    ResourceManager::generateExternalModels();

    Game *game = Game::getInstance();

    InputController::init(WindowController::window);



    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();




    double dt_step = 0.008f;


    while (!glfwWindowShouldClose(WindowController::window)) {
        // Clear the screen

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;



        while (deltaTime > dt_step) {
            game->checkCollisions();
            game->update(dt_step);
            deltaTime -= dt_step;
        }

        game->checkCollisions();
        game->update(deltaTime);


        game->draw();


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