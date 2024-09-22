#pragma once

#include "Node.h"
#include "Macros.h"

namespace BSplineCurves3D
{
    class Light : public Node
    {
    public:
        explicit Light(QObject* parent = nullptr);
        virtual ~Light();

        DEFINE_MEMBER(QVector4D, Color);
        DEFINE_MEMBER(float, Ambient);
        DEFINE_MEMBER(float, Diffuse);
        DEFINE_MEMBER(float, Specular);
    };
}
