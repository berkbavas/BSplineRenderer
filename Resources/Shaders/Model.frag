#version 450 core

struct Model
{
    vec4 color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct Light
{
    vec4 color;
    vec3 direction;
    float ambient;
    float diffuse;
    float specular;
};

uniform vec3 cameraPosition;
uniform Model model;
uniform Light light;

in vec4 fs_Position;
in vec3 fs_Normal;

layout(location = 0) out vec4 out_Color;

void main()
{
    // Ambient
    float ambient = light.ambient * model.ambient;

    // Diffuse
    float diffuse = max(dot(fs_Normal, light.direction), 0.0) * light.diffuse * model.diffuse;

    // Specular
    vec3 viewDirection = normalize(cameraPosition - fs_Position.xyz);
    vec3 reflectDirection = reflect(-light.direction, fs_Normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), model.shininess) * light.specular * model.specular;

    // Combine
    out_Color = (ambient + diffuse + specular) * model.color * light.color;
}
