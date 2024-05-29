#ifndef RENDERER_H
#define RENDERER_H


#include <glew.h>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include "controller/WindowController.h"
#include "objects/physics/Object.h"
#include "objects/graphics/Light.h"
#include "Shader.h"

class Renderer {

private:

    static inline GLuint VBO = 0;
    static inline GLuint VAO = 0;
    static inline GLuint EBO = 0;
    static inline GLuint textureCoordinateVBO = 0;
    static inline GLuint instanceModelMatrixVBO = 0;
    static inline GLuint colorVertexVBO = 0;
    static inline GLuint shadowMap = 0;
    static inline GLuint shadowMapFBO = 0;
    static inline GLfloat nearPlane = 0.1f;
    static inline GLfloat farPlane = 500.0f;
    static const GLuint SHADOW_WIDTH = 1024;
    static const GLuint SHADOW_HEIGHT = 1024;

public:

    static inline glm::mat4 projectionMatrix = glm::mat4(1.0f);
    static inline glm::mat4 viewMatrix = glm::mat4(1.0f);
    static inline Shader* pointShadowShader = nullptr;
    static inline Shader* objectShader = nullptr;
    static inline Shader* skyboxShader = nullptr;


    static void initializeGL() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
//        glEnable(GL_FRAMEBUFFER_SRGB);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &textureCoordinateVBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &instanceModelMatrixVBO);
        glGenBuffers(1, &colorVertexVBO);

        glGenFramebuffers(1, &shadowMapFBO);

        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                         GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        pointShadowShader = new Shader("shaders/pointShadowVertex.glsl",
                                                     "shaders/pointShadowFragment.glsl",
                                                     "shaders/pointShadowGeometry.glsl");

        checkOpenGLError("initializeGL");
    }


    static void nextDrawMode();

    static void checkOpenGLError(const std::string &at);

    static void drawInstancesOfModel(const ModelData &modelData, std::vector<Object *> *pVector, Shader* shader,
                                     bool hitboxes = false, const std::string &passName = "main");



    static void createAndSetPerspectiveProjectionMatrix(int _windowWidth, int _windowHeight);



    static void
    setMaterial(glm::vec3 materialAmbient, glm::vec3 materialDiffuse, glm::vec3 materialSpecular, float shininess,
                Shader* shader);

    static void setMaterial(Material material, Shader* shader);

    static int drawMode;

    static void switchDrawMode();


    static void
    drawScene(Light *light, std::map<ModelTypes, std::vector<Object *>> sceneObjects, bool drawHitboxes = false);

    static void setLight(Light *light);
};


#endif //RENDERER_H
