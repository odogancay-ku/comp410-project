#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    FragColor = vec4(objectColor, 1.0f);
}