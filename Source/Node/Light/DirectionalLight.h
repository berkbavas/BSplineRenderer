#pragma once

#include "Node/Light/Light.h"

namespace BSplineRenderer
{
    class DirectionalLight : public Light
    {
      public:
        DirectionalLight() = default;

        DEFINE_MEMBER(QVector3D, Direction, QVector3D(0, 1, 0));
    };

    using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;
}