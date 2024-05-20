#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec4 ModelMatrix_Column0;
layout (location = 3) in vec4 ModelMatrix_Column1;
layout (location = 4) in vec4 ModelMatrix_Column2;
layout (location = 5) in vec4 ModelMatrix_Column3;
layout (location = 6) in vec3 vertexColor;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 Normal;
out vec3 Color;
out vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main()
{

    mat4 ModelMatrix = mat4(1.0);

    ModelMatrix[0] = ModelMatrix_Column0;
    ModelMatrix[1] = ModelMatrix_Column1;
    ModelMatrix[2] = ModelMatrix_Column2;
    ModelMatrix[3] = ModelMatrix_Column3;

    vec4 worldPosition = ModelMatrix * vec4(localPosition, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;

    FragPos = vec3(worldPosition);

    mat3 normalMatrix = mat3(transpose(inverse(ModelMatrix)));
    vec3 normal = normalize(normalMatrix * vertexNormal);

    Normal = normal;

    // Lighting calculations
    vec3 fragPos = vec3(worldPosition);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*160.0f);
    vec3 specular = light.specular * (spec * material.specular);

    // Combine results
    vec3 result = (ambient + diffuse + specular) * vertexColor;
    Color = result;
}