#include "visuals/rendering.h"
#include "physics/object.h"
#include "game/game.h"
#include "input/input.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Main function
int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        // Initialization failed
        return -1;
    }

    // Create a fullscreen window
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int windowWidth = mode->width;
    int windowHeight = mode->height;

    // Create a window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Bouncing Ball", nullptr, nullptr);
    if (!window)
    {
        // Window or OpenGL context creation failed
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        // GLEW initialization failed
        glfwTerminate();
        return -1;
    }

    // Create an instance of the Renderer class
    Renderer renderer("vertexShader.glsl", "fragmentShader.glsl");

    Game game;

    InputHandler inputHandler(window, game, renderer);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    // Set the perspective projection matrix
    renderer.createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);


    // Create the walls

    Object wall1;
    wall1.vertices = {
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(1.0f, -1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, 1.0f)
    };
    wall1.indices = {
        0, 1, 2, 0, 2, 3, // Front
        1, 5, 6, 1, 6, 2, // Right
        5, 4, 7, 5, 7, 6, // Back
        4, 0, 3, 4, 3, 7, // Left
        3, 2, 6, 3, 6, 7, // Top
        4, 5, 1, 4, 1, 0  // Bottom
    };

    wall1.normals = {
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    wall1.colors = {
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    game.addObject(wall1);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the objects
        game.checkCollisions();
        game.update(deltaTime);
        game.draw(renderer);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
}