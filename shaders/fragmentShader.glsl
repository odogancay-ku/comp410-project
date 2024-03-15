#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Color; // New input for the color

void main()
{
    FragColor = vec4(Color, 1.0f); // Use the passed color instead of a uniform
}