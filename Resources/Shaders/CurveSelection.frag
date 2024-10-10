#version 450 core

in vec3 fs_Position;
in vec3 fs_Normal;

uniform int curveIndex;

layout(location = 0) out ivec4 out_CurveInfo;

void main()
{
    out_CurveInfo = ivec4(curveIndex, 0, 0, 1);
}
