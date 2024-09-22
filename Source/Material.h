#pragma once

#include "Macros.h"

#include <QVector4D>

namespace BSplineCurves3D
{
    class Material
    {
    public:
        Material();

        DEFINE_MEMBER(float, Ambient);
        DEFINE_MEMBER(float, Diffuse);
        DEFINE_MEMBER(float, Specular);
        DEFINE_MEMBER(float, Shininess);
        DEFINE_MEMBER(QVector4D, Color);
    };
}
