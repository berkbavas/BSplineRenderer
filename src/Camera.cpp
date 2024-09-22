#include "Camera.h"

#include <QTransform>
#include <QtMath>

BSplineCurves3D::Camera::Camera(QObject *parent)
    : Node(parent)
    , mAspectRatio(1.0f)
    , mZNear(0.1f)
    , mZFar(10000.0f)
{
    SetVerticalFov(60.0f);
}

float BSplineCurves3D::Camera::GetHorizontalFov() const
{
    return mHorizontalFov;
}

void BSplineCurves3D::Camera::SetHorizontalFov(float newHorizontalFov)
{
    mHorizontalFov = newHorizontalFov;

    float halfHorizontalFovRadian = 0.5f * qDegreesToRadians(mHorizontalFov);
    float verticalFovRadian = 2 * atan(tan(halfHorizontalFovRadian) * mAspectRatio);

    mVerticalFov = qRadiansToDegrees(verticalFovRadian);
}

float BSplineCurves3D::Camera::GetVerticalFov() const
{
    return mVerticalFov;
}

void BSplineCurves3D::Camera::SetVerticalFov(float newVerticalFov)
{
    mVerticalFov = newVerticalFov;

    float halfVerticalFovRadian = 0.5f * qDegreesToRadians(mVerticalFov);
    float horizontalFovRadian = 2 * atan(tan(halfVerticalFovRadian) / mAspectRatio);

    mHorizontalFov = qRadiansToDegrees(horizontalFovRadian);
}


QMatrix4x4 BSplineCurves3D::Camera::GetProjectionMatrix() const
{
    QMatrix4x4 projection;
    projection.perspective(mVerticalFov, mAspectRatio, mZNear, mZFar);
    return projection;
}

QVector3D BSplineCurves3D::Camera::GetViewDirection() const
{
    return mRotation * QVector3D(0, 0, -1);
}

QMatrix4x4 BSplineCurves3D::Camera::GetViewMatrix() const
{
    QMatrix4x4 transformation;
    transformation.rotate(mRotation.conjugated());
    transformation.translate(-mPosition);
    return transformation;
}
