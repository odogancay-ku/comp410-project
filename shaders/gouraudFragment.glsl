#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 Color;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;


void main() {
    FragColor = vec4(Color, 1.0);
}
