#include "HW2.h"
#include "objects/physics/RubiksCube.h"
#include "renderer/Renderer.h"
#include "camera/Camera.h"


void HW2::setup() {

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f   , -10.0f);

    glm::vec3 lightAmbient = {0.8f, 0.8f, 0.8f};
    glm::vec3 lightDiffuse = {0.0f, 0.0f, 0.0f};
    glm::vec3 lightSpecular = {0.0f, 0.0f, 0.0f};

    Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);


    environment = new Earth();

    std::cout << "HW2 setup" << std::endl;

    rubiksCube = new RubiksCube(glm::vec3 (10.0f, 0.0f, 0.0f));

    std::cout << "RubiksCube created" << std::endl;

    rubiksCube->addCubes();

    std::cout << "Cubes added" << std::endl;

    for (int i = 0; i < 27; i++) {
        addObject(rubiksCube->cubes[i]);
    }

    collisionStick = new Object();

    collisionStick->position = glm::vec3(0.0f, 0.0f, 0.0f);
    collisionStick->stretch = glm::vec3(5.0f, 0.1f, 0.1f);
    collisionStick->canMove = true;
    collisionStick->applyPhysics = false;
    collisionStick->applyGravity = false;
    collisionStick->isHidden = true;
    collisionStick->setOnCollision([](glm::vec3 normal, Object *pObject) {
        std::cout << "Collision detected" << std::endl;
        std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
    });


    addObject(collisionStick);

}

void HW2::onUpdate(float dt) {

    rubiksCube->rotateRow(0, 0.1f);


    collisionStick->position = Camera::getActiveInstance()->position;

    // Calculate direction using yaw and pitch, x,y,z is rotation in degrees

    float yaw = Camera::getActiveInstance()->yaw;
    float pitch = Camera::getActiveInstance()->pitch;

    collisionStick->rotation = glm::vec3(0.0f, -yaw, pitch);

}

