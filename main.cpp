#include "Angel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

float fov = 45.0f; // Field of View
float aspectRatio;
float nearPlane = 10.0f;
float farPlane = 110.0f;
float boundingBoxWidth;
float boundingBoxHeight;

GLuint programID;

struct Position
{
    float x, y, z;
};

// Define object properties
struct Object
{
    float speed_x, speed_y, speed_z;
    float mass;
    int object_type;
    float inelasticity_factor;
    vec3 color;
    float radius;
    Position position;
};

Object createObject()
{
    // Fill in the details
}

void load3DModel(const std::string &filename, std::vector<float> &vertices, std::vector<unsigned int> &indices)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip the OFF header

    // Read the counts
    std::getline(file, line);
    std::istringstream iss(line);
    int vertexCount, faceCount, edgeCount;
    iss >> vertexCount >> faceCount >> edgeCount;

    // Read the vertices
    for (int i = 0; i < vertexCount; ++i)
    {
        std::getline(file, line);
        std::istringstream iss(line);
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    // Read the faces
    for (int i = 0; i < faceCount; ++i)
    {
        std::getline(file, line);
        std::istringstream iss(line);
        int indexCount;
        iss >> indexCount;
        for (int j = 0; j < indexCount; ++j)
        {
            unsigned int index;
            iss >> index;
            indices.push_back(index);
        }
    }

    file.close();
}

std::string readShaderSource(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    // Print the source
    std::cout << ss.str() << std::endl;
    return ss.str();
}

GLuint loadShaders()
{
    // Load and compile the shaders

    std::string vertexShaderSource = readShaderSource("vertexShader.glsl");
    std::string fragmentShaderSource = readShaderSource("fragmentShader.glsl");

    // Create a vertex shader object
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the vertex shader source code to the shader object
    const GLchar *vertexShaderSourceCStr = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    // Check if the vertex shader compiled successfully
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar *fragmentShaderSourceCStr = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create a shader program object
    GLuint shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the shaders together into the shader program
    glLinkProgram(shaderProgram);

    // Check if the shaders were linked successfully
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        // The shaders were not linked successfully
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    return shaderProgram;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
        {
            char letter = 'A' + (key - GLFW_KEY_A);
            std::cout << "Key pressed: " << letter << std::endl;
        }
    }
}

// Define the callback function
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        std::cout << "Mouse button pressed: " << button << std::endl;
    }
}

std::vector<Object> objects;

void updateObjects(float deltaTime)
{
    for (Object &obj : objects)
    {
        // Update the object's position based on its speed and the elapsed time
        // Assuming the speed is in units per second
        obj.position.x += obj.speed_x * deltaTime;
        obj.position.y += obj.speed_y * deltaTime;
        obj.position.z += obj.speed_z * deltaTime;


        // Check if the object has collided with the bounding box
        if (obj.position.x + obj.radius > boundingBoxWidth || obj.position.x - obj.radius < -1 * boundingBoxWidth)
        {
            obj.speed_x *= -1.0f;
        }
        if (obj.position.y + obj.radius > boundingBoxHeight || obj.position.y - obj.radius < -1 * boundingBoxHeight)
        {
            obj.speed_y *= -1.0f;
        }
        if (obj.position.z - obj.radius < -1*boundingBoxWidth+(-1*nearPlane) || obj.position.z + obj.radius > -1*nearPlane)
        {
            obj.speed_z *= -1.0f;
        }

        // print the position of the object
        std::cout << "Position: " << obj.position.x << ", " << obj.position.y << ", " << obj.position.z << std::endl;
    }
}

void drawCube(Position position, float radius, vec3 color)
{
    // Cube vertices
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f};

    // Cube indices
    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1};

    GLuint VAO, VBO, EBO;

    // Generate and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind the VBO for the vertices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO for the indices
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Set the color
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    // Save the current transformation matrix
    glPushMatrix();

    // Translate to the position of the cube
    // Get the location of the uniform variable
    GLint translationLoc = glGetUniformLocation(programID, "translation");

    // Set the value of the uniform variable
    glUniform3f(translationLoc, position.x, position.y, position.z);

    // Scale the cube
    glScalef(radius, radius, radius);

    // Get the location of the uniform variable
    GLint colorLoc = glGetUniformLocation(programID, "objectColor");

    // Set the value of the uniform variable
    glUniform3f(colorLoc, color.x, color.y, color.z);

    // Draw the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Restore the transformation matrix
    glPopMatrix();

    // Delete the VAO, VBO and EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawObjects()
{
    for (const Object &obj : objects)
    {
        // Depending on the object type, call the appropriate drawing function
        // Here, we're assuming that 0 represents a sphere and 1 represents a cube
        if (obj.object_type == 0)
        {
            // TODO: Implement the drawSphere function
        }
        else if (obj.object_type == 1)
        {
            drawCube(obj.position, obj.radius, obj.color);
        }
    }
}

Object createCube(float x, float y, float z, float speed_x, float speed_y, float speed_z, float mass, float inelasticity_factor, vec3 color, float radius)
{
    Object cube;
    cube.speed_x = speed_x;
    cube.speed_y = speed_y;
    cube.speed_z = speed_z;
    cube.mass = mass;
    cube.object_type = 1;
    cube.inelasticity_factor = inelasticity_factor;
    cube.color = color;
    cube.radius = radius;
    cube.position = {x, y, z};
    objects.push_back(cube);
    return cube;
}

void createAndSetPerspectiveProjectionMatrix(int windowWidth, int windowHeight)
{


    // Define projection matrix parameters
    aspectRatio = (float)windowWidth / (float)windowHeight;


    boundingBoxWidth = nearPlane * tan(fov * 0.5f * (M_PI / 180.0f)) * 2.0f;
    boundingBoxHeight = boundingBoxWidth / aspectRatio;

    // Print aspect ratio
    std::cout << "Aspect ratio: " << aspectRatio << std::endl;

    // Calculate the projection matrix
    float f = 1.0f / tan(fov * 0.5f * (M_PI / 180.0f));
    float rangeInv = 1.0f / (nearPlane - farPlane);

    float projectionMatrix[16] = {
        f / aspectRatio, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (nearPlane + farPlane) * rangeInv, -1.0f,
        0.0f, 0.0f, nearPlane * farPlane * rangeInv * 2.0f, 0.0f};

    // Pass the projection matrix to the shader program
    GLint projectionLoc = glGetUniformLocation(programID, "ProjectionMatrix");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);
}

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
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Bouncing Ball", NULL, NULL);
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

    // After creating and compiling your shaders
    programID = loadShaders();

    glUseProgram(programID);

    // Register the callback
    glfwSetKeyCallback(window, key_callback);

    // Register the callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // Set the perspective projection matrix
    createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);

    // Initialize the walls
    // Front Wall
    //createCube(0.0f, 0.0f, -nearPlane-boundingBoxWidth*3/2, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f), boundingBoxWidth);
    // Right Wall
    //createCube(boundingBoxWidth*2/2, 0.0f, -nearPlane-boundingBoxWidth*1/2, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f), boundingBoxWidth);


    // Initialize the objects
    createCube(0.0f, 0.0f, -20.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, vec3(1.0f, 0.0f, 0.0f), 1.0f);
    createCube(0.0f, 0.0f, -20.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, vec3(0.0f, 1.0f, 0.0f), 2.0f);
    createCube(0.0f, 0.0f, -20.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, vec3(0.0f, 0.0f, 1.0f), 3.0f);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the objects
        updateObjects(deltaTime);

        // Draw the objects
        drawObjects();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
}