#version 330 core
layout (location = 0) in mat4 ModelMatrix;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 Normal;
out vec3 Color;

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
    Normal = normal;
    Color = color;
}