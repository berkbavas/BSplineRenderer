#version 450 core

struct Model
{
    vec4 color;
    float ambient;
    float diffuse;
};

struct Light
{
    vec4 color;
    vec3 direction;
    float ambient;
    float diffuse;
};

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
    float diffuse = max(dot(fs_Normal, -light.direction), 0.0) * light.diffuse * model.diffuse;

    // Combine
    out_Color = (ambient + diffuse) * model.color * light.color;
}
