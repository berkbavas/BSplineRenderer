#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 fs_Position;
out vec3 fs_Normal;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
    fs_Position = modelMatrix * vec4(position, 1.0f);
    fs_Normal = normalMatrix * normal;

    gl_Position = viewProjectionMatrix * fs_Position;
}
