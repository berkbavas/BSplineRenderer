#include "Node.h"

#include <QtMath>

BSplineCurves3D::Node::Node(QObject* parent)
    : QObject(parent)
    , mPosition(0, 0, 0)
    , mScale(1, 1, 1)

{}

BSplineCurves3D::Node::~Node() {}

const QMatrix4x4& BSplineCurves3D::Node::Transformation() const
{
    return mTransformation;
}

void BSplineCurves3D::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
}

const QQuaternion& BSplineCurves3D::Node::Rotation() const
{
    return mRotation;
}

void BSplineCurves3D::Node::SetRotation(const QQuaternion& newRotation)
{
    mRotation = newRotation;
    UpdateTransformation();
}

const QVector3D& BSplineCurves3D::Node::Position() const
{
    return mPosition;
}

void BSplineCurves3D::Node::SetPosition(const QVector3D& newPosition)
{
    mPosition = newPosition;
    UpdateTransformation();
}

const QVector3D& BSplineCurves3D::Node::Scale() const
{
    return mScale;
}

void BSplineCurves3D::Node::SetScale(const QVector3D& newScale)
{
    mScale = newScale;
    UpdateTransformation();
}

void BSplineCurves3D::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
}