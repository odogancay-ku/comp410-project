#version 330 core
layout (location = 0) in vec4 ModelMatrix_Column0;
layout (location = 1) in vec4 ModelMatrix_Column1;
layout (location = 2) in vec4 ModelMatrix_Column2;
layout (location = 3) in vec4 ModelMatrix_Column3;
layout (location = 4) in vec4 localPosition;
layout (location = 5) in vec4 vertexNormal;
layout (location = 6) in vec4 vertexColor;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 Normal;
out vec3 Color;

void main()
{

    mat4 ModelMatrix = mat4(1.0);
    
    ModelMatrix[0] = ModelMatrix_Column0;
    ModelMatrix[1] = ModelMatrix_Column1;
    ModelMatrix[2] = ModelMatrix_Column2;
    ModelMatrix[3] = ModelMatrix_Column3;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(localPosition);
    Normal = vertexNormal;
    Color = vertexColor;
}