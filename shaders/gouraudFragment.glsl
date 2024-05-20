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
in vec2 UV;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;
uniform sampler2D textureUnit0;


void main() {
    vec3 texColor = texture(textureUnit0, UV).rgb;
    vec3 result = Color*texColor;
    FragColor = vec4(result, 1.0);
}
