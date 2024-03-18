#include "renderer/Renderer.h"
#include <GLFW/glfw3.h>
#include "controller/WindowController.h"
#include "camera/Camera.h"
#include "controller/InputController.h"
#include "game/Game.h"


// Main function
int main() {

    WindowController windowController = WindowController::getInstance();

    windowController.createFullscreenWindow("Homework 1");
    GLfloat windowWidth = windowController.getWidth();
    GLfloat windowHeight = windowController.getHeight();
    GLFWwindow *window = windowController.getActiveWindow();


    std::cout << "Window created with size: " << windowWidth << "x" << windowHeight << std::endl;

    auto* renderer = new Renderer();
    Renderer::setActiveInstance(renderer);

    renderer->initializeGL();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    GLuint shaderProgram = renderer->loadShaderProgram("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    renderer->useShaderProgram(shaderProgram);

    std::cout << "Shader program loaded " << renderer->shaderProgram << std::endl;

    auto* camera = new Camera();
    Camera::setActiveInstance(camera);

    renderer->createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);



    Game* game = Game::getInstance();


    InputController inputController = InputController(window);

    ResourceManager::generateBuiltinModels();
    ResourceManager::generateExternalModels();

    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();


    std::cout << "WELCOME! Press P to give a little velocity to non static objects" << std::endl;
    std::cout << "Press C to create a new cube at origin with random speed and color" << std::endl;
    std::cout << "Press V to create a new sphere at origin with random speed and color" << std::endl;
    std::cout << "Press B to create a new bunny at origin with random speed and color" << std::endl;
    std::cout << "Press T to change the draw mode" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
    std::cout << "Press W, A, S, D, left shift, space to move the camera relative to the orientation" << std::endl;
    std::cout << "Press R to reset the camera" << std::endl;
    std::cout << "You can change the way you face using your mouse! Just drag it, no roll only yaw and pitch"
              << std::endl;
    std::cout
            << "For this to work we had to hide the mouse and fix it. "
               "Press alt to toggle this. "
               "You will need to test out the reshape callback functionality. "
               "Press alt, go out of full screen, "
               "either resize with the mouse or press alt again and keep pressing I and O. "
               "This will be enough to showcase window resize callback"
            << std::endl;
    std::cout << "Use the mouse wheel to change the fov" << std::endl;
    std::cout << "Have fun!" << std::endl;

    double dt_step = 0.008f;

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