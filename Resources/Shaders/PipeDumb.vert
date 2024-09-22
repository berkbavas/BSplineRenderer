#version 430 core
layout (location = 0) in float t; // [0,..., 1]

out float gs_t; // Pass t to the geometry shader

void main()
{
    gs_t = t;
}

