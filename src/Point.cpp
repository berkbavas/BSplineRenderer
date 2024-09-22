#include "Point.h"
#include "Curve.h"

BSplineCurves3D::Point::Point(QObject *parent)
    : QObject(parent)
    , mSelected(false)
    , mPosition(0, 0, 0)
{}

BSplineCurves3D::Point::Point(float x, float y, float z, QObject *parent)
    : QObject(parent)
    , mSelected(false)
    , mPosition(x, y, z)
{}

BSplineCurves3D::Point::Point(const QVector3D &position, QObject *parent)
    : QObject(parent)
    , mSelected(false)
    , mPosition(position)
{}

bool BSplineCurves3D::Point::GetSelected() const
{
    return mSelected;
}

void BSplineCurves3D::Point::SetSelected(bool newSelected)
{
    mSelected = newSelected;
}

const QVector3D &BSplineCurves3D::Point::GetPosition() const
{
    return mPosition;
}

void BSplineCurves3D::Point::SetPosition(const QVector3D &newPosition)
{
    mPosition = newPosition;

    Curve *curve = dynamic_cast<Curve *>(parent());

    if (curve)
        curve->SetDirty(true);
}
