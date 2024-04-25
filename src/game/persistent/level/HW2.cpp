#include "HW2.h"
#include "objects/physics/RubiksCube.h"
#include "renderer/Renderer.h"
#include "camera/Camera.h"
#include "controller/InputController.h"
#include "objects/physics/TightRope.h"
#include <random>

void HW2::setup() {

    glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 10.0f);

    glm::vec3 lightAmbient = {0.4f, 0.4f, 0.4f};
    glm::vec3 lightDiffuse = {0.8f, 0.8f, 0.8f};
    glm::vec3 lightSpecular = {0.3f, 0.3f, 0.3f};

    Renderer::getActiveInstance()->setLight(lightPos, lightAmbient, lightDiffuse, lightSpecular);


    environment = new Earth();

    std::cout << "HW2 setup" << std::endl;

    rubiksCube = new RubiksCube(glm::vec3(10.0f, 0.0f, 0.0f));
    rubiksCube2X2 = new RubiksCube2x2(glm::vec3(5.0f, 0.0f, 0.0f));

    std::cout << "RubiksCube created" << std::endl;

    rubiksCube->addCubes();
    rubiksCube2X2->addCubes();

    std::cout << "Cubes added" << std::endl;

    for (int i = 0; i < 27; i++) {
        addObject(rubiksCube->cubes[i]);
    }

    for (int i = 0; i < 8; i++) {
        addObject(rubiksCube2X2->cubes[i]);
    }

    collisionStick = new Object();

    collisionStick->position = glm::vec3(0.0f, 0.0f, 0.0f);
    collisionStick->stretch = glm::vec3(10.0f, 0.01f, 0.01f);
    collisionStick->canMove = true;
    collisionStick->applyPhysics = false;
    collisionStick->applyGravity = false;
    collisionStick->isHidden = true;
    collisionStick->setOnCollision([this](glm::vec3 normal, Object *pObject) {
        glm::vec3 rayOrigin = collisionStick->position;

        // Calculate direction of the ray from collisionStick rotation angles


        glm::vec3 rayDirection = collisionStick->orientation * glm::vec3(1.0f, 0.0f, 0.0f);


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
    hitMark->color = glm::vec3(1.0f, 0.0f, 1.0f);
    hitMark->isHidden = true;
    hitMark->applyPhysics = false;
    hitMark->applyGravity = false;
    hitMark->canMove = false;
    hitMark->canCollide = false;

    addObject(hitMark);

    pullMark = new Object();

    pullMark->modelType = ModelTypes::SPHERE;
    pullMark->scale = 0.1f;
    pullMark->color = glm::vec3(0.0f, 1.0f, 1.0f);
    pullMark->isHidden = true;
    pullMark->applyPhysics = false;
    pullMark->applyGravity = false;
    pullMark->canMove = false;
    pullMark->canCollide = false;

    addObject(pullMark);


    for (auto &cube: rubiksCube->cubes) {
        originalStartPositions.emplace_back(cube->position, cube->orientation);
    }

    for (auto &cube: rubiksCube2X2->cubes) {
        originalStartPositions2x2.emplace_back(cube->position, cube->orientation);
    }


    tightRope = new TightRope(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.0f), 0.03f, 100, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f));
    tightRope->isHidden = true;
    addObject(tightRope);

}

void HW2::onUpdate(float dt) {

    int dt_floor = (int) rotationQueueAnimationTime;
    int dt_floor2x2 = (int) rotationQueueAnimationTime2x2;

    if (playForward) {
        for (int i = 0; i < rotationQueue.size(); i++) {

            if (rotationState < i) {
                break;
            }

            auto tuple = rotationQueue[i];

            if (dt_floor > i) {
                continue;
            }

            auto [col, axis] = tuple;

            glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);

            if (axis == 0) {
                rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (axis == 1) {
                rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
            } else if (axis == 2) {
                rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
            }

            rubiksCube->rotateColumn(col, 90.0f * dt * playSpeed, rotationAxis);

            rotationQueueAnimationTime += dt * playSpeed;

            if ((int) rotationQueueAnimationTime > rotationState) {
                std::cout << "Played forward " << col << " " << axis << std::endl;
                rotationState++;
                rubiksCube->finishRotation();
            }

            if (rotationState == rotationQueue.size()) {
                playForward = false;
                for (auto &cube: rubiksCube->cubes) {
                    originalFinalPositions.emplace_back(cube->position, cube->orientation);
                }

            }

            break;

        }
    }

    if (playBack) {

        for (int i = 0; i < rotationQueue.size(); i++) {

            if (rotationState < i) {
                break;
            }

            auto tuple = rotationQueue[rotationQueue.size() - i - 1];

            if (dt_floor > i) {
                continue;
            }

            auto [col, axis] = tuple;

            glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);

            if (axis == 0) {
                rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (axis == 1) {
                rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
            } else if (axis == 2) {
                rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
            }

            rubiksCube->rotateColumn(col, -90.0f * dt * playSpeed, rotationAxis);

            rotationQueueAnimationTime += dt * playSpeed;

            if ((int) rotationQueueAnimationTime > rotationState) {
                std::cout << "Played backward " << col << " " << axis << std::endl;
                rotationState++;
                rubiksCube->finishRotation();
            }

            if (rotationState == rotationQueue.size()) {
                playBack = false;

                originalFinalPositions.clear();

            }

            break;

        }
    }

    if (!playForward && !playBack && InputController::keys[GLFW_KEY_N] && !originalFinalPositions.empty()) {
        playBack = true;
        rubiksCube->finishRotation();
        trackedCube = nullptr;

        rotationQueueAnimationTime = 0.0f;

        for (int i = 0; i < 27; i++) {
            auto &cube = rubiksCube->cubes[i];
            cube->position = originalFinalPositions[i].first;
            cube->orientation = originalFinalPositions[i].second;
        }

        rotationState = 0;

    }

    if (!playForward && !playBack && InputController::keys[GLFW_KEY_M]) {

        rotationQueue.clear();
        originalFinalPositions.clear();

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> col_dist(0, 2);
        std::uniform_int_distribution<int> axis_dist(0, 2);


        for (int i = 0; i < shuffleCount; i++) {
            int col = col_dist(gen);
            int axis = axis_dist(gen);

            rotationQueue.emplace_back(col, axis);
        }

        playForward = true;
        rubiksCube->finishRotation();
        trackedCube = nullptr;

        rotationQueueAnimationTime = 0.0f;

        for (int i = 0; i < 27; i++) {
            auto &cube = rubiksCube->cubes[i];
            cube->position = originalStartPositions[i].first;
            cube->orientation = originalStartPositions[i].second;
        }

        rotationState = 0;
    }

    if (playForward2x2) {
        for (int i = 0; i < rotationQueue2x2.size(); i++) {

            if (rotationState2x2 < i) {
                break;
            }

            auto tuple = rotationQueue2x2[i];

            if (dt_floor2x2 > i) {
                continue;
            }

            auto [col, axis] = tuple;

            glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);

            if (axis == 0) {
                rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (axis == 1) {
                rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
            } else if (axis == 2) {
                rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
            }

            rubiksCube2X2->rotateColumn(col, 90.0f * dt * playSpeed2x2, rotationAxis);

            rotationQueueAnimationTime2x2 += dt * playSpeed2x2;

            if ((int) rotationQueueAnimationTime2x2 > rotationState2x2) {
                std::cout << "Played forward " << col << " " << axis << " " << rotationState2x2 << " " << i << std::endl;
                rotationState2x2++;
                rubiksCube2X2->finishRotation();
            }

            if (rotationState2x2 == rotationQueue2x2.size()) {
                playForward2x2 = false;
                for (auto &cube: rubiksCube2X2->cubes) {
                    originalFinalPositions2x2.emplace_back(cube->position, cube->orientation);
                }

            }

            break;

        }
    }

    if (playBack2x2) {

        for (int i = 0; i < rotationQueue2x2.size(); i++) {

            if (rotationState2x2 < i) {
                break;
            }

            auto tuple = rotationQueue2x2[rotationQueue2x2.size() - i - 1];

            if (dt_floor2x2 > i) {
                continue;
            }

            auto [col, axis] = tuple;

            glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);

            if (axis == 0) {
                rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (axis == 1) {
                rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
            } else if (axis == 2) {
                rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
            }

            rubiksCube2X2->rotateColumn(col, -90.0f * dt * playSpeed2x2, rotationAxis);

            rotationQueueAnimationTime2x2 += dt * playSpeed2x2;

            if ((int) rotationQueueAnimationTime2x2 > rotationState2x2) {
                std::cout << "Played backward " << col << " " << axis << std::endl;
                rotationState2x2++;
                rubiksCube2X2->finishRotation();
            }

            if (rotationState2x2 == rotationQueue2x2.size()) {
                playBack2x2 = false;

                originalFinalPositions2x2.clear();

            }

            break;

        }
    }

    if (!playForward2x2 && !playBack2x2 && InputController::keys[GLFW_KEY_J] && !originalFinalPositions2x2.empty()) {
        playBack2x2 = true;
        rubiksCube2X2->finishRotation();
        trackedCube = nullptr;

        rotationQueueAnimationTime2x2 = 0.0f;

        for (int i = 0; i < 8; i++) {
            auto &cube = rubiksCube2X2->cubes[i];
            cube->position = originalFinalPositions2x2.at(i).first;
            cube->orientation = originalFinalPositions2x2.at(i).second;
        }

        rotationState2x2 = 0;

    }

    if (!playForward2x2 && !playBack2x2 && InputController::keys[GLFW_KEY_K]) {

        rotationQueue2x2.clear();
        originalFinalPositions2x2.clear();

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> col_dist(0, 9);
        std::uniform_int_distribution<int> axis_dist(0, 2);


        for (int i = 0; i < shuffleCount2x2; i++) {
            int col = col_dist(gen) > 5 ? 1 : 0;
            int axis = axis_dist(gen);

            std::cout << "col: " << col << " axis " << axis << std::endl;

            rotationQueue2x2.emplace_back(col, axis);
        }

        playForward2x2 = true;
        rubiksCube2X2->finishRotation();
        trackedCube = nullptr;

        rotationQueueAnimationTime2x2 = 0.0f;

        for (int i = 0; i < 8; i++) {
            std::cout << "Reset cube: " << i << std::endl;
            auto &cube = rubiksCube2X2->cubes[i];
            cube->position = originalStartPositions2x2.at(i).first;
            cube->orientation = originalStartPositions2x2.at(i).second;
        }

        rotationState2x2 = 0;
    }


    collisionStick->position = Camera::getActiveInstance()->position;

    // Calculate direction using yaw and pitch, x,y,z is rotation in degrees

    float yaw = Camera::getActiveInstance()->yaw;
    float pitch = Camera::getActiveInstance()->pitch;

    collisionStick->orientation = Object::pitchYawRollToQuat(glm::vec3(0.0f, -1 * yaw, pitch));


    if (InputController::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && !playForward && !playBack &&
        !playForward2x2 && !playBack2x2) {

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
            trackedCubeHoldStartPosition = closestLocalPoint;
            trackedCubePullMarkDistance = glm::distance(Camera::getActiveInstance()->position, closestPoint);


        }

    }

    if (trackedCube != nullptr) {

        hitMark->position = trackedCube->getModelMatrix() * glm::vec4(trackedCubeHoldPosition, 1.0f);
        hitMark->isHidden = false;

        // Calculate the direction the camera is looking at, place the pull mark at the distance of the tracked cube
        // from the camera


        glm::vec3 rayDirection = collisionStick->orientation * glm::vec3(1.0f, 0.0f, 0.0f);

        pullMark->position = Camera::getActiveInstance()->position + rayDirection * trackedCubePullMarkDistance;

        pullMark->isHidden = false;

        tightRope->isHidden = false;
        tightRope->updateEnds(hitMark->position, pullMark->position);

        // Determine if tracked cube belongs to 3x3 cube or 2x2 cube

        bool tracking2x2 = false;

        for (auto &cube: rubiksCube2X2->cubes) {

            if (cube == trackedCube) {
                tracking2x2 = true;
                break;
            }

        }

        if (tracking2x2) {

            rubiksCube2X2->determineAndStartRotation(trackedCube, hitMark->position, pullMark->position);

            rubiksCube2X2->updateRotation(dt, hitMark->position, pullMark->position);

            if (InputController::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
                rubiksCube2X2->finishRotation();
                hitMark->isHidden = true;
                pullMark->isHidden = true;
                tightRope->isHidden = true;
                trackedCube = nullptr;
            }

        } else {

            rubiksCube->determineAndStartRotation(trackedCube, hitMark->position, pullMark->position);

            rubiksCube->updateRotation(dt, hitMark->position, pullMark->position);

            if (InputController::mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
                rubiksCube->finishRotation();
                hitMark->isHidden = true;
                pullMark->isHidden = true;
                tightRope->isHidden = true;
                trackedCube = nullptr;
            }
        }


    }


    candidateCollisions.clear();


}


