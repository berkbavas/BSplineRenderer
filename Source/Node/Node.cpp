#include "Node.h"

void BSplineRenderer::Node::SetRotation(const QQuaternion& newRotation)
{
    if (mRotation == newRotation)
    {
        return;
    }

    mRotation = newRotation;
    Node::UpdateTransformation();
    MakeDirty();
}

void BSplineRenderer::Node::SetPosition(const QVector3D& newPosition)
{
    if (mPosition == newPosition)
    {
        return;
    }

    mPosition = newPosition;
    Node::UpdateTransformation();
    MakeDirty();
}

void BSplineRenderer::Node::SetScale(const QVector3D& newScale)
{
    if (mScale == newScale)
    {
        return;
    }

    mScale = newScale;
    Node::UpdateTransformation();
    MakeDirty();
}

void BSplineRenderer::Node::SetTransformation(const QMatrix4x4& newTransformation)
{
    if (mTransformation == newTransformation)
    {
        return;
    }

    mTransformation = newTransformation;
    mPosition = mTransformation.column(3).toVector3D();
    mRotation = QQuaternion::fromRotationMatrix(mTransformation.normalMatrix());
    MakeDirty();
}

void BSplineRenderer::Node::SetPosition(float x, float y, float z)
{
    SetPosition(QVector3D(x, y, z));
}

void BSplineRenderer::Node::SetScale(float x, float y, float z)
{
    SetScale(QVector3D(x, y, z));
}

void BSplineRenderer::Node::UpdateTransformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));

    mNormalMatrix = mTransformation.normalMatrix();
}

void BSplineRenderer::Node::RotateGlobal(const QVector3D& axis, float angle)
{
    SetRotation(QQuaternion::fromAxisAndAngle(axis, angle) * GetRotation());
}

void BSplineRenderer::Node::RotateLocal(const QVector3D& axis, float angle)
{
    SetRotation(GetRotation() * QQuaternion::fromAxisAndAngle(axis, angle));
}

void BSplineRenderer::Node::Translate(const QVector3D& delta)
{
    SetPosition(mPosition + delta);
}
