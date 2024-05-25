//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef COMP410_PROJECT_HW3_H
#define COMP410_PROJECT_HW3_H

#include <iostream>
#include "objects/physics/Object.h"
#include "game/persistent/environment/Earth.h"
#include "Level.h"
#include "renderer/Renderer.h"
#include "controller/InputController.h"

struct Light {
    glm::vec3 lightPos;
    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;

} typedef;

class HW3 : public Level {


public:

    Light *light;

    Object *dumbObject;

    int modelIndex = 0;
    int textureIndex = 0;
    int materialIndex = 0;

    std::vector<ModelData *> models;
    std::vector<GLuint> textures;
    std::vector<Material> materials;

    void nextModel() {
        modelIndex = (modelIndex + 1) % models.size();
        dumbObject->modelData = models[modelIndex];
        dumbObject->modelData->texture = textures[textureIndex];
    }

    void nextTexture() {
        textureIndex = (textureIndex + 1) % textures.size();
        dumbObject->modelData->texture = textures[textureIndex];
    }

    void nextMaterial() {
        materialIndex = (materialIndex + 1) % materials.size();
        dumbObject->modelData->material = materials[materialIndex];
    }

    HW3() {

        std::cout << "Creating models" << std::endl;

        ModelData *cubeModel = generateCubeModelData();
        auto *sphereModel = new ModelData();
        generateSphereModelData(sphereModel, 6);
        auto *bunnyModel = new ModelData(*ResourceManager::getModel(ModelTypes::BUNNY));
        auto *maidModel = new ModelData(*ResourceManager::getModel(ModelTypes::MAID));

        cubeModel->type = ModelTypes::UNIQUE_MODEL;
        sphereModel->type = ModelTypes::UNIQUE_MODEL;
        bunnyModel->type = ModelTypes::UNIQUE_MODEL;
        maidModel->type = ModelTypes::UNIQUE_MODEL;

        std::cout << "Generating texture coordinates" << std::endl;

        generateTextureCoordinatesByCubicProjection(cubeModel);
        generateTextureCoordinatesBySphericalProjection(sphereModel);
        generateTextureCoordinatesBySphericalProjection(bunnyModel);
        generateTextureCoordinatesBySphericalProjection(maidModel);

        ResourceManager::setModelHitbox(cubeModel);
        ResourceManager::setModelHitbox(sphereModel);
        ResourceManager::setModelHitbox(bunnyModel);
        ResourceManager::setModelHitbox(maidModel);

        models.push_back(cubeModel);
        models.push_back(sphereModel);
        models.push_back(bunnyModel);
        models.push_back(maidModel);

        textures.push_back(createBaseTexture());
        textures.push_back(createTextureFromPPM("assets/textures/basketball.ppm"));
        textures.push_back(createTextureFromPPM("assets/textures/earth.ppm"));

        cubeModel->texture = textures[0];
        sphereModel->texture = textures[0];
        bunnyModel->texture = textures[0];
        maidModel->texture = textures[0];

        materials.push_back(ResourceManager::whitePlastic);
        materials.push_back(ResourceManager::gold);
        materials.push_back(ResourceManager::silver);

        cubeModel->material = materials[0];
        sphereModel->material = materials[0];
        bunnyModel->material = materials[0];
        maidModel->material = materials[0];

    }

    static inline std::vector<GLuint> shaderPrograms = {};

    static inline std::vector<char *> vertexShaderPaths = {
            "shaders/blinnPhongVertex.glsl",
            "shaders/gouraudVertex.glsl",
            "shaders/phongVertex.glsl"
    };

    static inline std::vector<char *> fragmentShaderPaths = {
            "shaders/blinnPhongFragment.glsl",
            "shaders/gouraudFragment.glsl",
            "shaders/phongFragment.glsl"
    };

    static inline int shaderIndex = 0;

    void setup() override {

        std::cout << "Creating persistent HW3" << std::endl;


        light = new Light;

        float roomSize = 15;


//        glm::vec3 lightPos = glm::vec3(roomSize, roomSize / 4, 0.0f);
        glm::vec3 lightPos = glm::vec3(0.0f, roomSize/4   , 0.0f);

        glm::vec3 lightAmbient = {0.3f, 0.3f, 0.3f};
//        glm::vec3 lightAmbient = {1.0f, 1.0f, 1.0f};
        glm::vec3 lightDiffuse = {1.0f, 1.0f, 1.0f};
        glm::vec3 lightSpecular = {0.2f, 0.2f, 0.2f};

        light->lightPos = lightPos;
        light->lightAmbient = lightAmbient;
        light->lightDiffuse = lightDiffuse;
        light->lightSpecular = lightSpecular;

        Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);


        std::cout << "Level created" << std::endl;

        std::cout << "Creating environment" << std::endl;

        environment = new Earth();

        std::cout << "Environment created" << std::endl;

        std::cout << "Adding objects" << std::endl;


        auto *cube = new Object();
        cube->modelType = ModelTypes::UNIQUE_MODEL;
        cube->modelData = generateCubeModelData(glm::vec3(1.0f, 1.0f, 1.0f));
        cube->position = glm::vec3(roomSize, -roomSize / 2, 0.0f);
        cube->canMove = false;
        cube->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        cube->paint(0.2f, 0.2f, 0.4f);
        addObject(cube);

        auto *cube2 = new Object();
        cube2->modelType = ModelTypes::UNIQUE_MODEL;
        cube2->modelData = generateCubeModelData(glm::vec3(1.0f, 0.0f, 1.0f));
        cube2->position = glm::vec3(roomSize, +roomSize / 2, 0.0f);
        cube2->canMove = false;
        cube2->stretch = glm::vec3(roomSize, 1.0f, roomSize);
        addObject(cube2);

        auto *cube3 = new Object();
        cube3->modelType = ModelTypes::UNIQUE_MODEL;
        cube3->modelData = generateCubeModelData(glm::vec3(1.0f, 0.0f, 0.0f));
        cube3->position = glm::vec3(roomSize, 0.0f, -roomSize / 2 - 0.5);
        cube3->canMove = false;
        cube3->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        addObject(cube3);

        auto *cube4 = new Object();
        cube4->modelType = ModelTypes::UNIQUE_MODEL;
        cube4->modelData = generateCubeModelData(glm::vec3(1.0f, 0.0f, 0.0f));
        cube4->position = glm::vec3(roomSize, 0.0f, +roomSize / 2 + 0.5);
        cube4->canMove = false;
        cube4->stretch = glm::vec3(roomSize, roomSize + 1, 1.0f);
        addObject(cube4);

        auto *cube5 = new Object();
        cube5->modelType = ModelTypes::UNIQUE_MODEL;
        cube5->modelData = generateCubeModelData(glm::vec3(1.0f, 1.0f, 0.0f));
        cube5->position = glm::vec3(roomSize * 3 / 2 + 0.5, 0.0f, 0.0f);
        cube5->canMove = false;
        cube5->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube5->paint(0.4f, 0.2f, 0.2f);
        addObject(cube5);

        auto *cube6 = new Object();
        cube6->modelType = ModelTypes::UNIQUE_MODEL;
        cube6->modelData = generateCubeModelData(glm::vec3(0.0f, 1.0f, 1.0f));
        cube6->position = glm::vec3(roomSize / 2 - 0.5, 0.0f, 0.0f);
        cube6->canMove = false;
        cube6->stretch = glm::vec3(1.0f, roomSize + 1, roomSize + 2);
        cube6->isHidden = true;
        addObject(cube6);

        dumbObject = new Object();
        dumbObject->modelType = ModelTypes::UNIQUE_MODEL;
        dumbObject->position = glm::vec3(roomSize, -roomSize / 2 + 0.5f, -4.0f);
        dumbObject->velocity = glm::vec3(3.0f, 2.0f, 4.0f);
        dumbObject->angularVelocity = glm::vec3(0.0f, 1.0f, 1.0f);
        dumbObject->canMove = true;
        dumbObject->canRotate = true;
        dumbObject->scale = 3.0;
        dumbObject->mass = 1.0;
        dumbObject->applyGravity = false;
        dumbObject->restitution = 1.0;
        dumbObject->paint(1.0f, 1.0f, 1.0f);
        addObject(dumbObject);

        dumbObject->modelData = models[0];


        std::cout << "WELCOME! Press P to give a little velocity to non static objects" << std::endl;
        std::cout << "Press L to change the object model" << std::endl;
        std::cout << "Press H to show hitboxes, you might need to switch to point or line draw mode to see them"
                  << std::endl;
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
                   "resize with the mouse "
                   "This will be enough to showcase window resize callback"
                << std::endl;
        std::cout << "Use the mouse wheel to change the fov" << std::endl;
        std::cout << "Have fun!" << std::endl;

        for (int i = 0; i < vertexShaderPaths.size(); i++) {
            shaderPrograms.push_back(
                    Renderer::getActiveInstance()->loadShaderProgram(vertexShaderPaths[i], fragmentShaderPaths[i]));
        }


        InputController::addKeyEventAdapter(new KeyEventAdapter(

                [this](int key) {
                    switch (key) {
                        case GLFW_KEY_C:
                            shaderIndex = (shaderIndex + 1) % vertexShaderPaths.size();
                            std::cout << "Loading shader program: " << shaderPrograms[shaderIndex] << std::endl;
                            Renderer::getActiveInstance()->useShaderProgram(shaderPrograms[shaderIndex]);
                            Renderer::getActiveInstance()->setLight(light->lightPos, light->lightAmbient,
                                                                    light->lightDiffuse, light->lightDiffuse);

                            break;
                        case GLFW_KEY_V:
                            nextModel();
                            break;
                        case GLFW_KEY_B:
                            nextTexture();
                            break;
                        case GLFW_KEY_N:
                            nextMaterial();
                            break;
                        case GLFW_KEY_M:
                            // If the object is moving set it to rooms center, if it is not give it random velocity and angular velocity
                            if (dumbObject->canMove) {
                                dumbObject->position = glm::vec3(15.0f, 0.0f, 0.0f);
                                dumbObject->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
                                dumbObject->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
                                dumbObject->angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
                                dumbObject->canMove = false;
                            } else {
                                dumbObject->velocity = glm::vec3(3.0f, 2.0f, 4.0f);
                                dumbObject->angularVelocity = glm::vec3(0.0f, 1.0f, 1.0f);
                                dumbObject->canMove = true;
                            }
                            break;
                        case GLFW_KEY_J:
                            Renderer::nextDrawMode();
                            break;
                        case GLFW_KEY_H:
                            Game::getInstance()->drawHitboxes = !Game::getInstance()->drawHitboxes;
                            break;
                        default :
                            break;
                    }
                },
                [](int key) {
                },
                [](int key) {
                }

        ));

    }

    void onUpdate(float dt) override {

    }

};


#endif //COMP410_PROJECT_HW3_H
