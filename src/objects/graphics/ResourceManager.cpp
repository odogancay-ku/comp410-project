//
// Created by ofaru on 16.03.2024.
//

#include <complex>
#include <glm/geometric.hpp>
#include "ResourceManager.h"


void ResourceManager::generateBuiltinModels() {

    baseTexture = createBaseTexture();

    ModelData *cubeModelData = new ModelData();
    cubeModelData->type = ModelTypes::CUBE;
    cubeModelData->material = ResourceManager::whitePlastic;
    cubeModelData->texture = baseTexture;

    cubeModelData->vertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    cubeModelData->indices = {
            0, 2, 1, 2, 0, 3,  // front face
            1, 6, 5, 6, 1, 2,  // right face
            7, 5, 6, 5, 7, 4,  // back face
            4, 3, 0, 3, 4, 7,  // left face
            3, 6, 2, 6, 3, 7,  // top face
            4, 1, 5, 1, 4, 0   // bottom face
    };

    cubeModelData->normals = {
            glm::vec3(-1.0f, -1.0f, -1.0f), // Front bottom left
            glm::vec3(1.0f, -1.0f, -1.0f),  // Front bottom right
            glm::vec3(1.0f, 1.0f, -1.0f),   // Front top right
            glm::vec3(-1.0f, 1.0f, -1.0f),  // Front top left
            glm::vec3(-1.0f, -1.0f, 1.0f),  // Back bottom left
            glm::vec3(1.0f, -1.0f, 1.0f),   // Back bottom right
            glm::vec3(1.0f, 1.0f, 1.0f),    // Back top right
            glm::vec3(-1.0f, 1.0f, 1.0f)    // Back top left
    };

    cubeModelData->colorVertices = {
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    };

    ResourceManager::addModel(ModelTypes::CUBE, cubeModelData);

    ModelData *sphereModelData = new ModelData();
    sphereModelData->type = ModelTypes::SPHERE;
    sphereModelData->material = ResourceManager::whitePlastic;
    sphereModelData->texture = createTextureFromPPM("assets/textures/earth.ppm");

    ModelData *cylinderModelData = new ModelData();
    cylinderModelData->type = ModelTypes::CYLINDER;

    ModelData *coneModelData = new ModelData();
    coneModelData->type = ModelTypes::CONE;

    ModelData *torusModelData = new ModelData();
    torusModelData->type = ModelTypes::TORUS;

    ModelData *teapotModelData = new ModelData();
    teapotModelData->type = ModelTypes::TEAPOT;

    ModelData *planeModelData = new ModelData();
    planeModelData->type = ModelTypes::PLANE;

    ModelData *skyboxModelData = new ModelData();
    skyboxModelData->type = ModelTypes::SKYBOX;

    ModelData *quadModelData = new ModelData();
    quadModelData->type = ModelTypes::QUAD;


    generateSphere(sphereModelData->vertices, sphereModelData->normals, sphereModelData->indices,
                   sphereModelData->textureCoordinates, 5);


    std::cout << "Sphere vertices: " << sphereModelData->vertices.size() << std::endl;
    std::cout << "Sphere normals: " << sphereModelData->normals.size() << std::endl;
    std::cout << "Sphere indices: " << sphereModelData->indices.size() << std::endl;

    glm::vec3 lastColor;

    for (int i = 0; i < sphereModelData->vertices.size(); ++i) {
        // Generate a random color that is close to the last one
        float r = (float) rand() / RAND_MAX;
        float g = (float) rand() / RAND_MAX;
        float b = (float) rand() / RAND_MAX;

        if (i > 0) {
            r = (r + 9 * lastColor.x) / 10;
            g = (g + 9 * lastColor.y) / 10;
            b = (b + 9 * lastColor.z) / 10;
        }

        glm::vec3 color = glm::vec3(r, g, b);

        sphereModelData->colorVertices.push_back(color);

        lastColor = color;
    }

    ResourceManager::addModel(ModelTypes::SPHERE, sphereModelData);


}

void ResourceManager::generateExternalModels() {


    ModelData *bunnyModelData = new ModelData();
    bunnyModelData->type = ModelTypes::BUNNY;
    bunnyModelData->material = ResourceManager::gold;

    ResourceManager::loadModel("assets/models/bunny.off", bunnyModelData);


    ModelData *maidModelData = new ModelData();
    maidModelData->type = ModelTypes::MAID;
    maidModelData->material = ResourceManager::silver;

    ResourceManager::loadModel("assets/models/maid.off", maidModelData);

}


void divideTriangle(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textureCoordinates, const glm::vec3 &a,
                    const glm::vec3 &b, const glm::vec3 &c,
                    int depth) {
    if (depth == 0) {

        glm::vec3 vec1 = a / 2.f;
        glm::vec3 vec2 = b / 2.f;
        glm::vec3 vec3 = c / 2.f;

        vertices.push_back(vec1);
        vertices.push_back(vec2);
        vertices.push_back(vec3);

        // Push texture coordinates

        textureCoordinates.push_back(sphericalProjection(vec1));
        textureCoordinates.push_back(sphericalProjection(vec2));
        textureCoordinates.push_back(sphericalProjection(vec3));
    } else {
        // Calculate midpoints
        glm::vec3 ab = glm::normalize(glm::vec3{(a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2});
        glm::vec3 ac = glm::normalize(glm::vec3{(a.x + c.x) / 2, (a.y + c.y) / 2, (a.z + c.z) / 2});
        glm::vec3 bc = glm::normalize(glm::vec3{(b.x + c.x) / 2, (b.y + c.y) / 2, (b.z + c.z) / 2});

        // Recursively subdivide
        divideTriangle(vertices, textureCoordinates, a, ab, ac, depth - 1);
        divideTriangle(vertices, textureCoordinates, ab, b, bc, depth - 1);
        divideTriangle(vertices, textureCoordinates, ac, bc, c, depth - 1);
        divideTriangle(vertices, textureCoordinates, ab, bc, ac, depth - 1);
    }
}

// Function to generate normals and indices
void generateSphere(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLuint> &indices,
                    std::vector<glm::vec2> &textureCoordinates,
                    int subdivisions) {
    // Initial vertices of a tetrahedron
    glm::vec3 v0 = glm::normalize(glm::vec3{1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v1 = glm::normalize(glm::vec3{-1.0f, 0.0f, -1.0f / std::sqrt(2.0f)});
    glm::vec3 v2 = glm::normalize(glm::vec3{0.0f, 1.0f, 1.0f / std::sqrt(2.0f)});
    glm::vec3 v3 = glm::normalize(glm::vec3{0.0f, -1.0f, 1.0f / std::sqrt(2.0f)});

    // Subdivide each face of the tetrahedron
    divideTriangle(vertices, textureCoordinates, v0, v2, v3, subdivisions);
    divideTriangle(vertices, textureCoordinates, v1, v3, v2, subdivisions);
    divideTriangle(vertices, textureCoordinates, v0, v3, v1, subdivisions);
    divideTriangle(vertices, textureCoordinates, v0, v1, v2, subdivisions);

    // Calculate normals
    for (auto vertex: vertices) {
        normals.push_back(glm::normalize(vertex));
    }

    // Generate indices
    for (size_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(static_cast<GLuint>(i));
    }
}

void ResourceManager::loadModel(const std::string &filePath, ModelData *modelData) {

    // Load model data from file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    std::string firstLine;
    std::getline(file, firstLine);
    if (firstLine != "OFF") {
        std::cout << firstLine.data() << std::endl;
        std::cout << "ERROR: File is not in OFF format" << std::endl;
        exit(1);
    }

    // Read the number of vertices, faces, and edges

    int numVertices, numFaces, numEdges;
    file >> numVertices >> numFaces >> numEdges;


    // Read the vertices
    for (int i = 0; i < numVertices; ++i) {
        float x, y, z;
        file >> x >> y >> z;

        modelData->vertices.emplace_back(x, y, z);
        modelData->normals.emplace_back(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 lastColor;

    for (int i = 0; i < modelData->vertices.size(); ++i) {
        // Generate a random color that is close to the last one
        float r = (float) rand() / RAND_MAX;
        float g = (float) rand() / RAND_MAX;
        float b = (float) rand() / RAND_MAX;

        if (i > 0) {
            r = (r + 3 * lastColor.x) / 4;
            g = (g + 3 * lastColor.y) / 4;
            b = (b + 3 * lastColor.z) / 4;
        }

        glm::vec3 color = glm::vec3(r, g, b);

        modelData->colorVertices.push_back(color);

        lastColor = color;
    }


    // Read the faces
    for (int i = 0; i < numFaces; ++i) {
        int numSides;
        file >> numSides;
        for (int j = 0; j < numSides; ++j) {
            GLuint index;
            file >> index;
            modelData->indices.push_back(index);
        }
    }

    // Calculate smallest bounding box
    glm::vec3 min = modelData->vertices[0];
    glm::vec3 max = modelData->vertices[0];

    for (auto vertex: modelData->vertices) {

        if (vertex.x < min.x) {
            min.x = vertex.x;
        }
        if (vertex.y < min.y) {
            min.y = vertex.y;
        }
        if (vertex.z < min.z) {
            min.z = vertex.z;
        }
        if (vertex.x > max.x) {
            max.x = vertex.x;
        }
        if (vertex.y > max.y) {
            max.y = vertex.y;
        }
        if (vertex.z > max.z) {
            max.z = vertex.z;
        }
    }


    float normalizingLength = glm::length(max - min) / sqrt(3);

    std::cout << "Min: " << min.x << " " << min.y << " " << min.z << std::endl;
    std::cout << "Max: " << max.x << " " << max.y << " " << max.z << std::endl;
    std::cout << "Normalizing length: " << normalizingLength << std::endl;

    // Center and normalize the model
    glm::vec3 center = (min + max) / 2.0f;

    for (auto &vertex: modelData->vertices) {
        vertex = (vertex - center) / normalizingLength;
    }


    // Calculate normals without duplicating shared vertices


    for (int i = 0; i < modelData->indices.size(); i += 3) {
        glm::vec3 a = modelData->vertices[modelData->indices[i]];
        glm::vec3 b = modelData->vertices[modelData->indices[i + 1]];
        glm::vec3 c = modelData->vertices[modelData->indices[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));

        modelData->normals[modelData->indices[i]] += normal;
        modelData->normals[modelData->indices[i + 1]] += normal;
        modelData->normals[modelData->indices[i + 2]] += normal;

    }




    // Store loaded model data in the ResourceManager
    ResourceManager::addModel(modelData->type, modelData);


}

// Helper Method To Normalize Object Sizes

void ResourceManager::normalizeObjectSize(ModelData *modelData) {
    // Calculate the smallest bounding box
    glm::vec3 min = modelData->vertices[0];
    glm::vec3 max = modelData->vertices[0];

    for (auto vertex: modelData->vertices) {
        if (vertex.x < min.x) {
            min.x = vertex.x;
        }
        if (vertex.y < min.y) {
            min.y = vertex.y;
        }
        if (vertex.z < min.z) {
            min.z = vertex.z;
        }
        if (vertex.x > max.x) {
            max.x = vertex.x;
        }
        if (vertex.y > max.y) {
            max.y = vertex.y;
        }
        if (vertex.z > max.z) {
            max.z = vertex.z;
        }
    }

    float normalizingLength = glm::length(max - min);

    // Center and normalize the model
    glm::vec3 center = (min + max) / 2.0f;

    for (auto &vertex: modelData->vertices) {
        vertex = (vertex - center) / normalizingLength;
    }
}

void ResourceManager::addModel(const int modelIndex, ModelData *modelData) {

    setModelHitbox(modelData);
    generateTextureCoordinatesBySphericalProjection(modelData);

    models[modelIndex] = modelData;

    std::cout << "Model added: " << modelIndex << std::endl;
    std::cout << "Model added: " << models[modelIndex]->type << std::endl;
    std::cout << "Model added: " << models[modelIndex]->indices.size() << std::endl;

}


ModelData *generateSphereModelData(int subdivisions) {
    auto *sphereModelData = new ModelData();
    sphereModelData->type = ModelTypes::UNIQUE_MODEL;
    sphereModelData->material = ResourceManager::whitePlastic;

    generateSphere(sphereModelData->vertices, sphereModelData->normals, sphereModelData->indices,
                   sphereModelData->textureCoordinates, subdivisions);

    glm::vec3 lastColor;

    for (int i = 0; i < sphereModelData->vertices.size(); ++i) {
        // Generate a random color that is close to the last one
        float r = (float) rand() / RAND_MAX;
        float g = (float) rand() / RAND_MAX;
        float b = (float) rand() / RAND_MAX;

        if (i > 0) {
            r = (r + 9 * lastColor.x) / 10;
            g = (g + 9 * lastColor.y) / 10;
            b = (b + 9 * lastColor.z) / 10;
        }

        glm::vec3 color = glm::vec3(r, g, b);

        sphereModelData->colorVertices.push_back(color);

        lastColor = color;
    }

    return sphereModelData;
}

ModelData *generateCubeModelData() {
    auto *cubeModelData = new ModelData();
    cubeModelData->type = ModelTypes::UNIQUE_MODEL;
    cubeModelData->material = ResourceManager::whitePlastic;

    cubeModelData->vertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    cubeModelData->indices = {
            0, 2, 1, 2, 0, 3,  // front face
            1, 6, 5, 6, 1, 2,  // right face
            7, 5, 6, 5, 7, 4,  // back face
            4, 3, 0, 3, 4, 7,  // left face
            3, 6, 2, 6, 3, 7,  // top face
            4, 1, 5, 1, 4, 0   // bottom face
    };

    cubeModelData->normals = {
            glm::vec3(-1.0f, -1.0f, -1.0f), // Front bottom left
            glm::vec3(1.0f, -1.0f, -1.0f),  // Front bottom right
            glm::vec3(1.0f, 1.0f, -1.0f),   // Front top right
            glm::vec3(-1.0f, 1.0f, -1.0f),  // Front top left
            glm::vec3(-1.0f, -1.0f, 1.0f),  // Back bottom left
            glm::vec3(1.0f, -1.0f, 1.0f),   // Back bottom right
            glm::vec3(1.0f, 1.0f, 1.0f),    // Back top right
            glm::vec3(-1.0f, 1.0f, 1.0f)    // Back top left
    };

    cubeModelData->colorVertices = {
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    };

    cubeModelData->hitboxVertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, -0.5f, -0.5f),
            glm::vec3(0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, 0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, -0.5f, 0.5f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.5f, 0.5f, 0.5f)
    };

    cubeModelData->hitboxIndices = {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            7, 6, 5, 5, 4, 7,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4
    };

    cubeModelData->hitboxNormals = std::vector<glm::vec3>(cubeModelData->hitboxVertices.size(),
                                                          glm::vec3(0.0f, 0.0f, 0.0f));


    generateTextureCoordinatesBySphericalProjection(cubeModelData);

    return cubeModelData;
}


ModelData *generateCubeModelData(glm::vec3 color) {
    ModelData *cubeModelData = generateCubeModelData();
    for (auto &colorVertice: cubeModelData->colorVertices) {
        colorVertice = color;
    }
    cubeModelData->textureCoordinates = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
    };
    cubeModelData->texture = ResourceManager::baseTexture;
    return cubeModelData;
}

void generateTextureCoordinatesBySphericalProjection(ModelData *modelData) {
    for (auto vertex: modelData->vertices) {
        modelData->textureCoordinates.emplace_back(sphericalProjection(vertex));
    }
}

glm::vec2 sphericalProjection(glm::vec3 vertex) {
    float theta = atan2(vertex.z, vertex.x);
    float phi = acos(vertex.y);

    float u = theta / (2 * M_PI);
    float v = phi / M_PI;

    return {u, v};
}

bool loadPPM(const char *filename, int &width, int &height, unsigned char *&data) {
    FILE *fd;
    int k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    fd = fopen(filename, "r");
    fscanf(fd, "%[^\n]", b);
    if (b[0] != 'P' || b[1] != '3') {
        printf("%s is not a PPM file!\n", filename);
        exit(0);
    }
    fscanf(fd, "%c", &c);
    while (c == '#') {
        fscanf(fd, "%[^\n]", b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }
    ungetc(c, fd);

    fscanf(fd, "%d %d %d", &width, &height, &k);
    printf("%d rows  %d columns  max value= %d\n", width, height, k);

    nm = height * width;
    data = static_cast<unsigned char *>(malloc(3 * sizeof(GLuint) * nm));

    for (i = nm; i > 0; i--) {
        fscanf(fd, "%d %d %d", &red, &green, &blue);
        (data)[3 * nm - 3 * i] = red;
        (data)[3 * nm - 3 * i + 1] = green;
        (data)[3 * nm - 3 * i + 2] = blue;
    }

    fclose(fd);

    return true;
}

GLuint createBaseTexture() {
    // Create a white texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    unsigned char data[3] = {255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint createTextureFromPPM(const char *filename) {
    int width, height;
    unsigned char *data;
    if (!loadPPM(filename, width, height, data)) {
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}