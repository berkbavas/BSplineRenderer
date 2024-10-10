#include "Knot.h"

BSplineRenderer::Knot::Knot(float x, float y, float z)
    : mPosition(x, y, z)
{
}

BSplineRenderer::Knot::Knot(const QVector3D& position)
    : mPosition(position)
{
}

void BSplineRenderer::Knot::SetPosition(const QVector3D& position)
{
    mPosition = position;
}

void BSplineRenderer::Knot::SetPosition(float x, float y, float z)
{
    mPosition.setX(x);
    mPosition.setY(y);
    mPosition.setZ(z);
}
