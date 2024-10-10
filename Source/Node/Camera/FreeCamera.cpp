#include "FreeCamera.h"

#include "Core/Constants.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <cmath>

const QMatrix4x4& BSplineRenderer::FreeCamera::GetProjectionMatrix()
{
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mVerticalFov, GetAspectRatio(), GetZNear(), GetZFar());
    return mProjectionMatrix;
}

const QMatrix4x4& BSplineRenderer::FreeCamera::GetViewProjectionMatrix()
{
    mViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
    return mViewProjectionMatrix;
}

const QMatrix4x4& BSplineRenderer::FreeCamera::GetViewMatrix()
{
    mViewMatrix.setToIdentity();
    mViewMatrix.rotate(GetRotation().conjugated());
    mViewMatrix.translate(-GetPosition());
    return mViewMatrix;
}

const QMatrix4x4& BSplineRenderer::FreeCamera::GetRotationMatrix()
{
    constexpr QVector4D ZERO_TRANSLATION(0, 0, 0, 1);
    mRotationMatrix = mViewMatrix;
    mRotationMatrix.setColumn(3, ZERO_TRANSLATION);
    return mRotationMatrix;
}

const QVector3D& BSplineRenderer::FreeCamera::GetViewDirection()
{
    constexpr QVector3D NEGATIVE_Z(0, 0, -1);
    mViewDirection = GetRotation() * NEGATIVE_Z;
    return mViewDirection;
}

void BSplineRenderer::FreeCamera::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
}

float BSplineRenderer::FreeCamera::GetHorizontalFov() const
{
    const auto hfov = std::atan(std::atan(mVerticalFov / 2.0) / GetAspectRatio()) * 2.0f;
    return std::abs(qRadiansToDegrees(hfov));
}

float BSplineRenderer::FreeCamera::GetAspectRatio() const
{
    return float(mWidth) / float(mHeight);
}

void BSplineRenderer::FreeCamera::Update(float ifps)
{
    // Rotation
    if (mUpdateRotation)
    {
        RotateGlobal(QVector3D(0, 1, 0), mAngularSpeedMultiplier * mAngularSpeed * mMouse.dx * ifps);
        RotateLocal(QVector3D(1, 0, 0), mAngularSpeedMultiplier * mAngularSpeed * mMouse.dy * ifps);
        mMouse.dx = 0.0f;
        mMouse.dy = 0.0f;
        mUpdateRotation = false;
    }

    // Translation
    if (mUpdatePosition)
    {
        if (mPressedKeys[Qt::Key_Control])
        {
            mLinearSpeed = 100.0f;
        }
        else if (mPressedKeys[Qt::Key_Shift])
        {
            mLinearSpeed = 10.0f;
        }
        else
        {
            mLinearSpeed = 1.0f;
        }

        const auto keys = mPressedKeys.keys();

        for (const auto key : keys)
        {
            if (mPressedKeys.value(key, false))
            {
                const auto binding = KEY_BINDINGS.value(key, QVector3D(0, 0, 0));
                const auto direction = GetRotation().rotatedVector(binding);
                const auto delta = mLinearSpeed * mLinearSpeedMultiplier * ifps * direction;
                Translate(delta);
            }
        }
    }

    if (mPressedKeys.empty())
    {
        mUpdatePosition = false;
    }
}

void BSplineRenderer::FreeCamera::Reset()
{
    const auto keys = mPressedKeys.keys();

    for (const auto key : keys)
    {
        mPressedKeys.insert(key, false);
    }

    mUpdatePosition = false;
    mUpdateRotation = false;
    mMouse.Reset();
}

void BSplineRenderer::FreeCamera::KeyPressed(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key) event->key(), true);
    mUpdatePosition = true;
}

void BSplineRenderer::FreeCamera::KeyReleased(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key) event->key(), false);
}

void BSplineRenderer::FreeCamera::MousePressed(QMouseEvent* event)
{
    mMouse.x = event->pos().x();
    mMouse.y = event->pos().y();
    mMouse.button = event->button();
}

void BSplineRenderer::FreeCamera::MouseReleased(QMouseEvent* event)
{
    if (mMouse.button == event->button())
    {
        mMouse.button = Qt::NoButton;
    }
}

void BSplineRenderer::FreeCamera::MouseMoved(QMouseEvent* event)
{
    if (mMouse.button == mActionReceiveButton)
    {
        mMouse.dx += mMouse.x - event->pos().x();
        mMouse.dy += mMouse.y - event->pos().y();

        mMouse.x = event->pos().x();
        mMouse.y = event->pos().y();
        mUpdateRotation = true;
    }
}

QVector3D BSplineRenderer::FreeCamera::GetDirectionFromScreenCoodinates(int x, int y)
{
    const float halfVerticalFovRadian = 0.5f * qDegreesToRadians(mVerticalFov);
    const float halfHorizontalFovRadian = std::atan(std::tan(halfVerticalFovRadian) * GetAspectRatio());

    const float horizontalRotationAngleRadian = std::atan(std::tan(halfHorizontalFovRadian) * (0.5f * mWidth - x) / (0.5f * mWidth));
    const float horizontalRotationAngle = qRadiansToDegrees(horizontalRotationAngleRadian);

    const float verticalRotationAngleRadian = std::atan(((0.5f * mHeight - y) / (0.5f * mWidth - x)) * std::sin(horizontalRotationAngleRadian));
    const float verticalRotationAngle = qRadiansToDegrees(verticalRotationAngleRadian);

    QQuaternion left = GetRotation() * QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), horizontalRotationAngle);
    QQuaternion leftThenUp = left * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), verticalRotationAngle);

    return leftThenUp * QVector3D(0, 0, -1);
}

const QMap<Qt::Key, QVector3D> BSplineRenderer::FreeCamera::KEY_BINDINGS = //
    {
        { Qt::Key_W, QVector3D(0, 0, -1) },
        { Qt::Key_S, QVector3D(0, 0, 1) },
        { Qt::Key_A, QVector3D(-1, 0, 0) },
        { Qt::Key_D, QVector3D(1, 0, 0) },
        { Qt::Key_E, QVector3D(0, 1, 0) },
        { Qt::Key_Q, QVector3D(0, -1, 0) }
    };
