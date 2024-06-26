#version 330 core
layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec4 ModelMatrix_Column0;
layout (location = 3) in vec4 ModelMatrix_Column1;
layout (location = 4) in vec4 ModelMatrix_Column2;
layout (location = 5) in vec4 ModelMatrix_Column3;
layout (location = 6) in vec3 vertexColor;
layout (location = 7) in vec2 vertexUV;


void main()
{

    mat4 ModelMatrix = mat4(1.0);

    ModelMatrix[0] = ModelMatrix_Column0;
    ModelMatrix[1] = ModelMatrix_Column1;
    ModelMatrix[2] = ModelMatrix_Column2;
    ModelMatrix[3] = ModelMatrix_Column3;

    gl_Position = ModelMatrix * vec4(localPosition, 1.0);
}

