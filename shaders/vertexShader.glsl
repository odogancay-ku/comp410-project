#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix; // New uniform for the view matrix
uniform vec3 translation;

out vec3 Normal;

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPos + translation, 1.0); // Multiply with the view matrix
    Normal = normal;
}