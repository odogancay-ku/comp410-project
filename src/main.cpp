#include "Angel.h"
#include "visuals/utility.h"
#include "visuals/drawer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

float g = 9.8; // Acceleration due to gravity

float fov = 45.0f; // Field of View
float aspectRatio;
float nearPlane = 10.0f;
float farPlane = 110.0f;
float boundingBoxWidth;
float boundingBoxHeight;
float boundingBoxLeft;
float boundingBoxRight;
float boundingBoxBottom;
float boundingBoxTop;
float boundingBoxBack;
float boundingBoxFront;

GLuint programID;


// Define object properties
struct Object
{
    float speed_x, speed_y, speed_z;
    float mass;
    int object_type;
    float inelasticity_factor;
    vec3 color;
    float radius;
    vec3 position;
    bool doesCollide;
    bool doesMove;
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

        if (!obj.doesMove)
        {
            continue;
        }

        if (obj.mass > 0)
        {
            obj.speed_y -= g * deltaTime;
        }

        obj.position.x += obj.speed_x * deltaTime;
        obj.position.y += obj.speed_y * deltaTime;
        obj.position.z += obj.speed_z * deltaTime;

        if (obj.doesCollide)
        {

            float directionedRadiusX = obj.radius * (obj.speed_x > 0 ? 1 : -1);
            float directionedRadiusY = obj.radius * (obj.speed_y > 0 ? 1 : -1);
            float directionedRadiusZ = obj.radius * (obj.speed_z > 0 ? 1 : -1);

            if (obj.position.x + directionedRadiusX > boundingBoxRight && obj.speed_x > 0)
            {
                obj.position.x = boundingBoxRight - directionedRadiusX;
                obj.speed_x *= -obj.inelasticity_factor;
            }
            else if (obj.position.x + directionedRadiusX < boundingBoxLeft && obj.speed_x < 0)
            {
                obj.position.x = boundingBoxLeft - directionedRadiusX;
                obj.speed_x *= -obj.inelasticity_factor;
            }

            if (obj.position.y + directionedRadiusY > boundingBoxTop && obj.speed_y > 0)
            {
                obj.position.y = boundingBoxTop - directionedRadiusY;
                obj.speed_y *= -obj.inelasticity_factor;

                if (std::abs(obj.speed_y) < pow(10, -9))
                {
                    obj.speed_y = 0;
                }

            }
            else if (obj.position.y + directionedRadiusY < boundingBoxBottom && obj.speed_y < 0)
            {
                obj.position.y = boundingBoxBottom - directionedRadiusY;
                obj.speed_y *= -obj.inelasticity_factor;

                if (std::abs(obj.speed_y) < pow(10, -9))
                {
                    obj.speed_y = 0;
                }

            }

            if (obj.position.z + directionedRadiusZ > boundingBoxFront && obj.speed_z > 0)
            {
                obj.position.z = boundingBoxFront - directionedRadiusZ;
                obj.speed_z *= -obj.inelasticity_factor;
            }
            else if (obj.position.z + directionedRadiusZ < boundingBoxBack && obj.speed_z < 0)
            {
                obj.position.z = boundingBoxBack - directionedRadiusZ;
                obj.speed_z *= -obj.inelasticity_factor;
            }

            // Handle the collision with other objects

            // Remove infinitesimal speed
            if (std::abs(obj.speed_x) < pow(10, -9))
            {
                obj.speed_x = 0;
            }
            if (std::abs(obj.speed_z) < pow(10, -9))
            {
                obj.speed_z = 0;
            }
        }
    }
}



// Function to calculate length of a vector
GLfloat vec3Length(const vec3 &v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Function to normalize a vector
vec3 vec3Normalize(const vec3 &v)
{
    GLfloat len = vec3Length(v);
    return {v.x / len, v.y / len, v.z / len};
}

void drawCube(vec3 position, float radius, vec3 color)
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

    // Scale vertices
    for (int i = 0; i < 24; i += 3)
    {
        vertices[i] *= radius * 2;
        vertices[i + 1] *= radius * 2;
        vertices[i + 2] *= radius * 2;
    }

    GLfloat normals[] = {
        // Front face
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Back face
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Top face
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // Left face
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Right face
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f};

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

    // Copy vertex data to VBO
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Copy normal data to VBO
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);

    // Set the vertex attributes pointers for position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    // Set the vertex attributes pointers for normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(sizeof(vertices)));
    glEnableVertexAttribArray(1);

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

    // Get the location of the uniform variables
    GLint lightPosLoc = glGetUniformLocation(programID, "lightPos");
    GLint lightColorLoc = glGetUniformLocation(programID, "lightColor");
    GLint viewPosLoc = glGetUniformLocation(programID, "viewPos");

    // Set the values of the uniform variables
    vec3 lightPos = vec3(0, boundingBoxHeight, boundingBoxFront - boundingBoxWidth / 2);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 viewPos = vec3(0, 0, 0);

    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(viewPosLoc, viewPos.x, viewPos.y, viewPos.z);

    // Draw the cube
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    // Restore the transformation matrix
    glPopMatrix();

    glBindVertexArray(0);

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

Object createCube(float x, float y, float z, float speed_x, float speed_y, float speed_z, float mass, float inelasticity_factor, vec3 color, float radius, bool doesCollide = true, bool doesMove = true)
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
    cube.doesCollide = doesCollide;
    cube.doesMove = doesMove;
    objects.push_back(cube);
    return cube;
}

void createAndSetPerspectiveProjectionMatrix(int windowWidth, int windowHeight)
{

    // Define projection matrix parameters
    aspectRatio = (float)windowWidth / (float)windowHeight;

    boundingBoxWidth = nearPlane * tan(fov * 0.5f * (M_PI / 180.0f)) * 2.0f;
    boundingBoxHeight = boundingBoxWidth / aspectRatio;

    boundingBoxLeft = -boundingBoxWidth / 2;
    boundingBoxRight = boundingBoxWidth / 2;
    boundingBoxBottom = -boundingBoxHeight / 2;
    boundingBoxTop = boundingBoxHeight / 2;
    boundingBoxBack = -nearPlane - boundingBoxWidth;
    boundingBoxFront = -nearPlane;

    nearPlane = 9.9f;

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

    // After creating and compiling your shaders
    programID = loadShaders();

    glUseProgram(programID);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Register the callback
    glfwSetKeyCallback(window, key_callback);

    // Register the callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set the perspective projection matrix
    createAndSetPerspectiveProjectionMatrix(windowWidth, windowHeight);

    // Initialize the objects

    // Right Top Back Corner Cube
    createCube(boundingBoxWidth / 2 - 0.5, boundingBoxHeight / 2 - 0.5, -nearPlane - boundingBoxWidth + 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(1.0f, 0.0f, 0.0f), 0.5f, true, false);

    // Left Top Back Corner Cube
    createCube(-boundingBoxWidth / 2 + 0.5, boundingBoxHeight / 2 - 0.5, -nearPlane - boundingBoxWidth + 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(0.0f, 1.0f, 0.0f), 0.5f, true, false);

    // Right Bottom Back Corner Cube
    createCube(boundingBoxWidth / 2 - 0.5, -boundingBoxHeight / 2 + 0.5, -nearPlane - boundingBoxWidth + 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(0.0f, 0.0f, 1.0f), 0.5f, true, false);

    // Left Bottom Back Corner Cube
    createCube(-boundingBoxWidth / 2 + 0.5, -boundingBoxHeight / 2 + 0.5, -nearPlane - boundingBoxWidth + 0.5, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, vec3(0.5f, 1.0f, 0.5f), 0.5f, true, false);

    // Moving Cube
    createCube(1.0f, 0.0f, -nearPlane - 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.9f, vec3(0.5f, 0.5f, 0.5f), 0.5f);

    // Create 5 random cubes
    for (int i = 0; i < 5; i++)
    {
        float x = (rand() % (int)boundingBoxWidth) - boundingBoxWidth / 2;
        float y = (rand() % (int)boundingBoxHeight) - boundingBoxHeight / 2;
        float z = (rand() % (int)boundingBoxWidth) - boundingBoxWidth / 2;
        float speed_x = (rand() % 20) - 10;
        float speed_y = (rand() % 20) - 10;
        float speed_z = (rand() % 20) - 10;
        float mass = 1.0f;
        float inelasticity_factor = 0.9f;
        vec3 color = vec3((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
        float radius = 0.5f;
        createCube(x, y, z, speed_x, speed_y, speed_z, mass, inelasticity_factor, color, radius);
    }

    vec3 boundingBoxBottomLeftFront = {boundingBoxLeft, boundingBoxBottom, boundingBoxFront};
    vec3 boundingBoxBottomRightFront = {boundingBoxRight, boundingBoxBottom, boundingBoxFront};
    vec3 boundingBoxBottomLeftBack = {boundingBoxLeft, boundingBoxBottom, boundingBoxBack};
    vec3 boundingBoxBottomRightBack = {boundingBoxRight, boundingBoxBottom, boundingBoxBack};
    vec3 boundingBoxTopLeftFront = {boundingBoxLeft, boundingBoxTop, boundingBoxFront};
    vec3 boundingBoxTopRightFront = {boundingBoxRight, boundingBoxTop, boundingBoxFront};
    vec3 boundingBoxTopLeftBack = {boundingBoxLeft, boundingBoxTop, boundingBoxBack};
    vec3 boundingBoxTopRightBack = {boundingBoxRight, boundingBoxTop, boundingBoxBack};

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

        // Draw the bounding box walls
        drawRectangle(boundingBoxBottomLeftFront, boundingBoxBottomRightBack, vec3(0.0f, 1.0f, 1.0f));
        drawRectangle(boundingBoxTopLeftFront, boundingBoxTopRightBack, vec3(1.0f, 0.0f, 1.0f));
        drawRectangle(boundingBoxBottomLeftFront, boundingBoxTopLeftBack, vec3(1.0f, 1.0f, 1.0f));
        drawRectangle(boundingBoxBottomRightFront, boundingBoxTopRightBack, vec3(1.0f, 1.0f, 1.0f));
        drawRectangle(boundingBoxBottomLeftBack, boundingBoxTopRightBack, vec3(1.0f, 1.0f, 0.0f));

        // Draw the objects
        drawObjects();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
}