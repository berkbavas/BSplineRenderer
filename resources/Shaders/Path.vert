#version 430 core
layout (location = 0) in float t; // [0,...,1]

uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 control_points[16];
uniform int control_points_count;

const int degree = control_points_count - 1;

float custom_pow(float x, float y)
{
    if(x == 0 && y == 0)
        return 1;

    return pow(x, y);
}

float factorial(int n)
{
    float result = 1;

    for(int i = 2; i <= n; ++i)
        result *= i;

    return result;
}


float choose(int n, int k)
{
    return factorial(n) / (factorial(k) * factorial(n - k));
}


vec3 value_at(float t)
{
    vec3 value = vec3(0, 0, 0);

    for(int i = 0; i <= degree; ++i)
    {
        float c0 = choose(degree, i);
        float c1 = custom_pow(t, i);
        float c2 = custom_pow(1 - t, degree - i);

        value += c0 * c1 * c2 * control_points[i];
    }

    return value;
}


vec3 tangent_at(float t)
{
    vec3 tangent = vec3(0,0,0);

    for(int i = 0; i <= degree - 1; i++)
    {
        float c0 = choose(degree - 1, i);
        float c1 = custom_pow(t, i);
        float c2 = custom_pow(1 - t, degree - 1 - i);
        tangent += degree * c0 * c1 * c2 * (control_points[i+1] - control_points[i]);
    }

   return normalize(tangent);
}

void main()
{
    gl_Position = projection_matrix * view_matrix * vec4(value_at(t), 1.0);
}

