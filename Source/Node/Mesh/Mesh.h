#pragma once

#include "Core/Constants.h"
#include "Node/Mesh/Vertex.h"
#include "Util/Macros.h"

#include <QOpenGLExtraFunctions>
#include <memory>

namespace BSplineRenderer
{

    class Mesh : protected QOpenGLExtraFunctions
    {
        DISABLE_COPY(Mesh);

      public:
        Mesh() = default;
        virtual ~Mesh() = default;

        virtual void Render() = 0;
    };

    using MeshPtr = std::shared_ptr<Mesh>;
    using MeshWeakPtr = std::weak_ptr<Mesh>;
}