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

void main()
{

    mat4 ModelMatrix = mat4(1.0);

    ModelMatrix[0] = ModelMatrix_Column0;
    ModelMatrix[1] = ModelMatrix_Column1;
    ModelMatrix[2] = ModelMatrix_Column2;
    ModelMatrix[3] = ModelMatrix_Column3;

    vec4 worldPosition = ModelMatrix * vec4(localPosition, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(ModelMatrix)));


    gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
    FragPos = vec3(worldPosition);
    Normal = normalize(normalMatrix * vertexNormal);
    Color = vertexColor;
}