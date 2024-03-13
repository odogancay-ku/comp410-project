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

    Game game;

    Camera camera;

    InputHandler inputHandler(window, game, renderer, camera);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Set the perspective projection matrix
    renderer.createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);


    // Create the surfaces using renderer.boundingBox values

    std::vector<vec3> backVertices = {
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxBack)
    };

    std::vector<vec3> frontVertices = {
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxFront)
    };

    std::vector<vec3> leftVertices = {
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxBack)
    };

    std::vector<vec3> rightVertices = {
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxBack)
    };

    std::vector<vec3> topVertices = {
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxTop, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxTop, renderer.boundingBoxFront)
    };

    std::vector<vec3> bottomVertices = {
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxBack),
            vec3(renderer.boundingBoxRight, renderer.boundingBoxBottom, renderer.boundingBoxFront),
            vec3(renderer.boundingBoxLeft, renderer.boundingBoxBottom, renderer.boundingBoxFront)
    };

    Surface backSurface(backVertices, vec3(1.0f, 1.0f, 1.0f));
    Surface frontSurface(frontVertices, vec3(1.0f, 1.0f, 0.0f));
    Surface leftSurface(leftVertices, vec3(1.0f, 0.0f, 1.0f));
    Surface rightSurface(rightVertices, vec3(0.0f, 1.0f, 1.0f));
    Surface topSurface(topVertices, vec3(1.0f, 0.0f, 0.0f));
    Surface bottomSurface(bottomVertices, vec3(0.0f, 0.0f, 1.0f));

    game.addObject(backSurface);
    game.addObject(frontSurface);
    game.addObject(leftSurface);
    game.addObject(rightSurface);
    game.addObject(topSurface);
    game.addObject(bottomSurface);

    // Print back surface coordinates
    std::cout << "Back surface coordinates: " << std::endl;
    for (int i = 0; i < backSurface.vertices.size(); i++) {
        std::cout << backSurface.vertices[i] << std::endl;
    }
    std::cout << "Back surface position: " << backSurface.position << std::endl;

    // Print front surface coordinates
    std::cout << "Front surface coordinates: " << std::endl;
    for (int i = 0; i < frontSurface.vertices.size(); i++) {
        std::cout << frontSurface.vertices[i] << std::endl;
    }
    std::cout << "Front surface position: " << frontSurface.position << std::endl;

    InputHandler::camera.yaw = -90.0f;

    double lastTime = glfwGetTime();

    double frameCount = 0;
    double lastFPSTime = glfwGetTime();

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
        }


    }

    // Terminate GLFW
    glfwTerminate();
}