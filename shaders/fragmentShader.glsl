#version 330 core
out vec4 FragColor;

in vec4 Normal;
in vec4 Color;

void main()
{
    FragColor = Color;
}