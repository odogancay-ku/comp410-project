#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "controller/WindowController.h"
#include "camera/Camera.h"
#include "controller/InputController.h"
#include "game/Game.h"

// Main function
int main() {

    WindowController *windowController = WindowController::getInstance();

    windowController->createFullscreenWindow("Homework 1");
    GLfloat windowWidth = windowController->getWidth();
    GLfloat windowHeight = windowController->getHeight();
    GLFWwindow *window = windowController->getActiveWindow();


    std::cout << "Window created with size: " << windowWidth << "x" << windowHeight << std::endl;

    auto *renderer = new Renderer();
    Renderer::setActiveInstance(renderer);

    renderer->initializeGL();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

//    renderer->loadObjectShaderProgram(
//            "shaders/gouraudVertex.glsl",
//            "shaders/gouraudFragment.glsl"
//            );
    renderer->loadObjectShaderProgram(
            "shaders/blinnPhongVertex.glsl",
            "shaders/blinnPhongFragment.glsl"
            );


    auto *camera = new Camera();
    Camera::setActiveInstance(camera);
    renderer->useObjectShaderProgram();
    renderer->createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);




    ResourceManager::generateBuiltinModels();
    ResourceManager::generateExternalModels();

    Game *game = Game::getInstance();

    InputController::init(window);



    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();




    double dt_step = 0.008f;

    std::cout << "Starting game loop" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


        glfwSwapBuffers(window);
        glfwPollEvents();


        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }


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