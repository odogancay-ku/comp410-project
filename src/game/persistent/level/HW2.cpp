#include "HW2.h"
#include "objects/physics/RubiksCube.h"
#include "renderer/Renderer.h"
#include "camera/Camera.h"
#include "controller/InputController.h"


void HW2::setup() {

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -10.0f);

    glm::vec3 lightAmbient = {0.8f, 0.8f, 0.8f};
    glm::vec3 lightDiffuse = {0.0f, 0.0f, 0.0f};
    glm::vec3 lightSpecular = {0.0f, 0.0f, 0.0f};

    Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);


    environment = new Earth();

    std::cout << "HW2 setup" << std::endl;

    rubiksCube = new RubiksCube(glm::vec3(10.0f, 0.0f, 0.0f));

    std::cout << "RubiksCube created" << std::endl;

    rubiksCube->addCubes();

    std::cout << "Cubes added" << std::endl;

    for (int i = 0; i < 27; i++) {
        addObject(rubiksCube->cubes[i]);
    }

    collisionStick = new Object();

    collisionStick->position = glm::vec3(0.0f, 0.0f, 0.0f);
    collisionStick->stretch = glm::vec3(5.0f, 0.01f, 0.01f);
    collisionStick->canMove = true;
    collisionStick->applyPhysics = false;
    collisionStick->applyGravity = false;
    collisionStick->isHidden = true;
    collisionStick->setOnCollision([this](glm::vec3 normal, Object *pObject) {
        glm::vec3 rayOrigin = collisionStick->position;

        // Calculate direction of the ray from collisionStick rotation angles

        float yaw = collisionStick->rotation.y * -1;
        float pitch = collisionStick->rotation.z;

        float yawRad = glm::radians(yaw + 90.0f);
        float pitchRad = glm::radians(pitch);

        float x = std::cos(pitchRad) * std::sin(yawRad);
        float y = std::sin(pitchRad);
        float z = std::cos(pitchRad) * std::cos(yawRad);

        glm::vec3 rayDirection = glm::normalize(glm::vec3(x, y, -1 * z));

        glm::mat4 inverseModel = glm::inverse(pObject->getModelMatrix());

        glm::vec3 localRayOrigin = glm::vec3(inverseModel * glm::vec4(rayOrigin, 1.0f));
        glm::vec3 localRayDirection = glm::normalize(glm::vec3(inverseModel * glm::vec4(rayDirection, 0.0f)));

        glm::vec3 min = -glm::vec3(pObject->scale / 2);
        glm::vec3 max = glm::vec3(pObject->scale / 2);
        glm::vec3 invDir = 1.0f / localRayDirection;
        glm::vec3 tMin = (min - localRayOrigin) * invDir;
        glm::vec3 tMax = (max - localRayOrigin) * invDir;

        glm::vec3 t1 = glm::min(tMin, tMax);
        glm::vec3 t2 = glm::max(tMin, tMax);

        float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
        float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);


        if (tNear > tFar || tFar < 0.0f) {
            return;
        }

        glm::vec3 hitPoint = localRayOrigin + localRayDirection * tNear;

        glm::vec3 hitPointWorld = glm::vec3(pObject->getModelMatrix() * glm::vec4(hitPoint, 1.0f));


        std::tuple<glm::vec3, glm::vec3, Object *> candidate = std::make_tuple(hitPoint, hitPointWorld, pObject);


        candidateCollisions.push_back(candidate);

    });


    addObject(collisionStick);


    hitMark = new Object();

    hitMark->modelType = ModelTypes::SPHERE;
    hitMark->scale = 0.1f;
    hitMark->color = glm::vec3(1.0f, 0.3f, 1.0f);
    hitMark->isHidden = true;
    hitMark->applyPhysics = false;
    hitMark->applyGravity = false;
    hitMark->canMove = false;
    hitMark->canCollide = false;

    addObject(hitMark);

    pullMark = new Object();

    pullMark->modelType = ModelTypes::SPHERE;
    pullMark->scale = 0.1f;
    pullMark->color = glm::vec3(0.3f, 1.0f, 1.0f);
    pullMark->isHidden = true;
    pullMark->applyPhysics = false;
    pullMark->applyGravity = false;
    pullMark->canMove = false;
    pullMark->canCollide = false;

    addObject(pullMark);



}

void HW2::onUpdate(float dt) {

    rubiksCube->rotateRow(0, 0.1f);


    collisionStick->position = Camera::getActiveInstance()->position;

    // Calculate direction using yaw and pitch, x,y,z is rotation in degrees

    float yaw = Camera::getActiveInstance()->yaw;
    float pitch = Camera::getActiveInstance()->pitch;

    collisionStick->rotation = glm::vec3(0.0f, -yaw, pitch);

    if (InputController::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS) {

        float closestDistance = 1000000.0f;
        glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 closestLocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);
        Object *closestObject = nullptr;

        for (std::tuple<glm::vec3, glm::vec3, Object *> candidate: candidateCollisions) {

            auto [point, worldPoint, object] = candidate;

            float distance = glm::distance(Camera::getActiveInstance()->position, worldPoint);

            if (distance < closestDistance) {
                closestDistance = distance;
                closestPoint = worldPoint;
                closestLocalPoint = point;
                closestObject = object;
            }

        }

        if (closestObject != nullptr && trackedCube == nullptr) {

            // Do something with the closest object

            trackedCube = closestObject;
            trackedCubeHoldPosition = closestLocalPoint;
            trackedCubePullMarkDistance = glm::distance(Camera::getActiveInstance()->position, closestPoint);


        }

    }

    if (trackedCube != nullptr) {

        hitMark->position = trackedCube->getModelMatrix() * glm::vec4(trackedCubeHoldPosition, 1.0f);
        hitMark->isHidden = false;

        // Calculate the direction the camera is looking at, place the pull mark at the distance of the tracked cube
        // from the camera

        float yaw = collisionStick->rotation.y * -1;
        float pitch = collisionStick->rotation.z;

        float yawRad = glm::radians(yaw + 90.0f);
        float pitchRad = glm::radians(pitch);

        float x = std::cos(pitchRad) * std::sin(yawRad);
        float y = std::sin(pitchRad);
        float z = std::cos(pitchRad) * std::cos(yawRad);

        glm::vec3 rayDirection = glm::normalize(glm::vec3(x, y, -1 * z));

        pullMark->position = Camera::getActiveInstance()->position + rayDirection * trackedCubePullMarkDistance;

        pullMark->isHidden = false;

    }

    if (InputController::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {

        hitMark->isHidden = true;
        pullMark->isHidden = true;
        trackedCube = nullptr;


    }

    candidateCollisions.clear();


}

