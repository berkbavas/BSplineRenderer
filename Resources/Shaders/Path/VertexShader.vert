#version 430 core
layout (location = 0) in float vertex;

uniform mat4 projectionMatrix;
uniform vec4 color;
uniform vec3 controlPoints;
uniform int controlPointsCount;

out vec4 fs_Color;

float customPow(float x, float y)
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


vec3 valueAt(float t)
{
    vec3 value = vec3(0, 0);
    int degree = controlPointsCount - 1;

    for(int i = 0; i <= degree; ++i)
    {
        float c0 = choose(degree, i);
        float c1 = customPow(t, i);
        float c2 = customPow(1 - t, degree - i);

        value += c0 * c1 * c2 * controlPoints[i];
    }

    return value;
}


vec3 tangentAt(float t)
{
    vec3 tangent = vec3(0,0);
    int degree = controlPointsCount - 1;

    for(int i = 0; i <= degree - 1; i++)
    {
        float c0 = choose(degree - 1, i);
        float c1 = customPow(t, i);
        float c2 = customPow(1 - t, degree - 1 - i);
        tangent += degree * c0 * c1 * c2 * (controlPoints[i+1] - controlPoints[i]);
    }

   return normalize(tangent);
}

void main()
{
    gl_Position = projectionMatrix * vec4(valueAt(vertex), 1);
    fs_Color = color;
}

