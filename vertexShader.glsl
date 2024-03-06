#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 ProjectionMatrix;
uniform vec3 translation;

void main()
{
    gl_Position = ProjectionMatrix * vec4(aPos + translation, 1.0);
}