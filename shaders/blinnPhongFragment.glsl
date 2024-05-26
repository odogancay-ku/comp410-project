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

uniform samplerCube depthMap;
uniform float far_plane;

vec3 gridSamplingDisk[20] = vec3[]
(
vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float calculateShadowFactor()
{
//        vec3 fragToLight = FragPos - light.position;
//        float currentDepth = length(fragToLight);
//        float shadow = 0.0;
//        float bias = 0.05;
//        float samples = 4.0;
//        float offset = 0.1;
//        for (float x = -offset; x < offset; x += offset / (samples * 0.5))
//        {
//            for (float y = -offset; y < offset; y += offset / (samples * 0.5))
//            {
//                for (float z = -offset; z < offset; z += offset / (samples * 0.5))
//                {
//                    float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
//                    closestDepth *= far_plane;
//                    if (currentDepth - bias > closestDepth)
//                    shadow += 1.0;
//                }
//            }
//        }
//        shadow /= (samples * samples * samples);

    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for (int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;
        if (currentDepth - bias > closestDepth)
        shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

void main()
{
    vec3 norm = Normal;
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir+ viewDir);
    float distance = length(FragPos-light.position);

    float diff = max(dot(norm, lightDir), 0.0);

    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess*160.0f);
    //    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    if (diff == 0.0)
    spec = 0.0;

    float a = 1.0f;
    float b = 0.01f;
    float c = 0.01f;
    float attenuationFactor = (a+b*distance+c*pow(distance, 2));

    vec3 ambient  = light.ambient * material.ambient;
    vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 texColor = texture(textureUnit0, UV).rgb;

    float shadow = calculateShadowFactor();
    shadow = clamp(shadow, 0.0, 1.0);
    float nonShadow = 1.0 - shadow;

    vec3 nonAmbient = (diffuse+specular);
    vec3 nonShadowed = nonAmbient*nonShadow;
    vec3 all = ambient + nonShadowed;
    vec3 colored = all * Color;
    vec3 textured = colored * texColor;

    vec3 result = textured;

    //    FragColor = vec4(vec3(1-shadow), 1.0);
    FragColor = vec4(result, 1.0);

    return;
}