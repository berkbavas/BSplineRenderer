#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fs_position;
out vec3 fs_normal;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    fs_position = position;
    fs_normal =  normal;

    gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
}
