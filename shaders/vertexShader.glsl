#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 worldPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 Normal;
out vec3 Color;

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPos + worldPos, 1.0);
    Normal = normal;
    Color = aColor;
}