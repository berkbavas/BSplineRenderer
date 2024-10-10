#pragma once

#include <QVector2D>
#include <QVector3D>

namespace BSplineRenderer
{
    struct Vertex
    {
        QVector3D position;
        QVector3D normal;
        QVector2D textureCoords;
    };
}
