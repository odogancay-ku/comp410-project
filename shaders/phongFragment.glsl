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

void main()
{
    vec3 norm = Normal;
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*160.0f);

    if (diff == 0.0)
        spec = 0.0;

    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 texColor = texture(textureUnit0, UV).rgb;
    vec3 result = (ambient + diffuse + specular) * Color*texColor;

    FragColor = vec4(result, 1.0);
}