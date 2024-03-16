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
    frontWall.isHidden = true;
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

    Cube cube({0, 0, 0}, {0, 0, 1}, 1);
    cube.velocity = {3.0f, 1.0f, 2.0f};
    cube.isAffectedByGravity = false;
    cube.applyAcceleration = false;
    cube.restitution = 1.0f;
    game.addObject(cube);

    InputHandler::camera.yaw = -90.0f;
    InputHandler::camera.pitch = -0.0f;
    InputHandler::camera.position = {0.0f, 0.0f, renderer.boundingBoxWidth};

    renderer.setCamera(InputHandler::camera.position, InputHandler::camera.yaw, InputHandler::camera.pitch);

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
            game.checkCollisions();
            double step = std::min(deltaTime, dt_step);
            game.update(step);
            deltaTime -= step;
        }


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
        if (elapsedTime >= 5.0) {
            std::cout << "FPS: " << frameCount / elapsedTime << std::endl;
            frameCount = 0;
            lastFPSTime = currentFPSTime;

            // Print all the objects
//            for (auto &object: game.objects) {
//                std::cout << "Object id: " << object.id << std::endl;
//                std::cout << "Object position: " << object.position.x << " " << object.position.y << " " << object.position.z << std::endl;
//                std::cout << "Object velocity: " << object.velocity.x << " " << object.velocity.y << " " << object.velocity.z << std::endl;
//            }
//
//             Print memory usage
//            std::cout << "Memory usage: " << game.objects.size()*sizeof (Object) << std::endl;

        }


    }

    // Terminate GLFW
    glfwTerminate();
}