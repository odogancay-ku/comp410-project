#include "visuals/rendering.h"
#include "physics/object.h"
#include "game/game.h"
#include "input/input.h"

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        // Initialization failed
        return -1;
    }

    // Create a fullscreen window
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int windowWidth = mode->width;
    int windowHeight = mode->height;

    // Create a window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Magic Bunny", nullptr, nullptr);
    if (!window) {
        // Window or OpenGL context creation failed
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        // GLEW initialization failed
        glfwTerminate();
        return -1;
    }

    // Create an instance of the Renderer class
    Renderer renderer("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    renderer.windowWidth = windowWidth;
    renderer.windowHeight = windowHeight;

    Game game;

    Camera camera;

    InputHandler inputHandler(window, game, renderer, camera);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Set the perspective projection matrix
    renderer.createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight, 90.0f);


    // Create the surfaces using renderer.boundingBox values


    Cube backWall({0.0f, 0.0f, -renderer.boundingBoxWidth}, {0.5f, 0.5f, 0.0f}, renderer.boundingBoxWidth);
    backWall.isStatic = true;
    game.addObject(backWall);

    Cube frontWall({0.0f, 0.0f, renderer.boundingBoxWidth}, {0.0f, 0.0f, 0.5f}, renderer.boundingBoxWidth);
    frontWall.isStatic = true;
    game.addObject(frontWall);

    Cube leftWall({-renderer.boundingBoxWidth, 0.0f, 0.0f}, {0.5f, 0.0f, 0.5f}, renderer.boundingBoxWidth);
    leftWall.isStatic = true;
    game.addObject(leftWall);

    Cube rightWall({renderer.boundingBoxWidth, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, renderer.boundingBoxWidth);
    rightWall.isStatic = true;
    game.addObject(rightWall);

    Cube bottomWall({0.0f, -renderer.boundingBoxWidth, 0.0f}, {0.5f, 0.5f, 0.5f}, renderer.boundingBoxWidth);
    bottomWall.isStatic = true;
    game.addObject(bottomWall);

    Cube topWall({0.0f, renderer.boundingBoxWidth, 0.0f}, {0.5f, 0.5f, 0.5f}, renderer.boundingBoxWidth);
    topWall.isStatic = true;
    game.addObject(topWall);

    float relativeSpeed = 15.0f;

    Cube cube1({-1.2f, 0, 1.0f}, {1.0f, 0.0f, 0.0f}, 1.0f);
    cube1.isAffectedByGravity = false;
    cube1.velocity = {relativeSpeed/2, 0, 0};
    cube1.restitution = 1.0f;
    game.addObject(cube1);

    Cube cube2({1.2f, 0, 1.0f}, {0.0f, 1.0f, 0.0f}, 1.0f);
    cube2.isAffectedByGravity = false;
    cube2.velocity = {-relativeSpeed/2, 0, 0};
    cube2.restitution = 1.0f;
    game.addObject(cube2);



    InputHandler::camera.yaw = -90.0f;
    InputHandler::camera.pitch = -0.0f;
    InputHandler::camera.position = {0.0f, 0.0f, renderer.boundingBoxWidth/2};

    renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);

    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();

    // Print all the objects
    for (auto &object: game.objects) {
        std::cout << "Object id: " << object.id << std::endl;
        std::cout << "Object position: " << object.position.x << " " << object.position.y << " " << object.position.z << std::endl;
        std::cout << "Object velocity: " << object.velocity.x << " " << object.velocity.y << " " << object.velocity.z << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
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

        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }

        frameCount++;
        double currentFPSTime = glfwGetTime();
        double elapsedTime = currentFPSTime - lastFPSTime;
        if (elapsedTime >= 1.0) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastFPSTime = currentFPSTime;

            // Print all the objects
            for (auto &object: game.objects) {
                std::cout << "Object id: " << object.id << std::endl;
                std::cout << "Object position: " << object.position.x << " " << object.position.y << " " << object.position.z << std::endl;
                std::cout << "Object velocity: " << object.velocity.x << " " << object.velocity.y << " " << object.velocity.z << std::endl;
            }

        }


    }

    // Terminate GLFW
    glfwTerminate();
}