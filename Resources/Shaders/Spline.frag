#version 450 core

struct Curve
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

uniform Curve curve;
uniform Light light;

in vec3 fs_Position;
in vec3 fs_Normal;

layout(location = 0) out vec4 out_Color;

void main()
{
    // Ambient
    float ambient = light.ambient * curve.ambient;

    // Diffuse
    float diffuse = max(dot(fs_Normal, light.direction), 0.0) * light.diffuse * curve.diffuse;

    // Combine
    out_Color = (ambient + diffuse) * curve.color * light.color;
}
