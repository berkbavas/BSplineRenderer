#version 450 core

struct Curve
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

    // Specular
    vec3 viewDirection = normalize(cameraPosition - fs_Position);
    vec3 reflectDirection = reflect(-light.direction, fs_Normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), curve.shininess) * light.specular * curve.specular;

    // Combine
    out_Color = (ambient + diffuse + specular) * curve.color * light.color;
}
