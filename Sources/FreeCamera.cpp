#include "FreeCamera.h"

#include <QTransform>
#include <QtMath>

BSplineCurves3D::FreeCamera::FreeCamera(QObject* parent)
    : Camera(parent)
    , mMovementSpeed(5.0f)
    , mAngularSpeed(25.0f)
    , mMousePressed(false)
    , mMousePreviousX(0.0f)
    , mMousePreviousY(0.0f)
    , mMouseDeltaX(0.0f)
    , mMouseDeltaY(0.0f)
    , mUpdateRotation(true)
    , mUpdatePosition(true)
{}

void BSplineCurves3D::FreeCamera::KeyPressed(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key)event->key(), true);
    mUpdatePosition = true;
}

void BSplineCurves3D::FreeCamera::KeyReleased(QKeyEvent* event)
{
    mPressedKeys.insert((Qt::Key)event->key(), false);
}

void BSplineCurves3D::FreeCamera::MousePressed(QMouseEvent* event)
{
    mMousePreviousX = event->x();
    mMousePreviousY = event->y();
    mMousePressed = true;
}

void BSplineCurves3D::FreeCamera::MouseReleased(QMouseEvent*)
{
    mMousePressed = false;
}

void BSplineCurves3D::FreeCamera::MouseMoved(QMouseEvent* event)
{
    if (mMousePressed)
    {
        mMouseDeltaX += mMousePreviousX - event->x();
        mMouseDeltaY += mMousePreviousY - event->y();

        mMousePreviousX = event->x();
        mMousePreviousY = event->y();
        mUpdateRotation = true;
    }
}

void BSplineCurves3D::FreeCamera::Update(float ifps)
{
    // Rotation
    if (mUpdateRotation)
    {
        mRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), mAngularSpeed * mMouseDeltaX * ifps) * mRotation;
        mRotation = mRotation * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), mAngularSpeed * mMouseDeltaY * ifps);

        mMouseDeltaY = 0.0f;
        mMouseDeltaX = 0.0f;
        mUpdateRotation = false;
    }

    // Translation
    if (mUpdatePosition)
    {
        const QList<Qt::Key> keys = mPressedKeys.keys();

        if (mPressedKeys[Qt::Key_Shift])
            mMovementSpeed = 50.0f;
        else if (mPressedKeys[Qt::Key_Control])
            mMovementSpeed = 1.0f;
        else
            mMovementSpeed = 5.0f;

        for (auto key : keys)
            if (mPressedKeys.value(key, false))
                mPosition += mMovementSpeed * ifps * mRotation.rotatedVector(KEY_BINDINGS.value(key, QVector3D(0, 0, 0)));
    }

    if (mPressedKeys.empty())
    {
        mUpdatePosition = false;
    }
}

const QMap<Qt::Key, QVector3D> BSplineCurves3D::FreeCamera::KEY_BINDINGS = {
    {Qt::Key_W, QVector3D(0, 0, -1)},
    {Qt::Key_S, QVector3D(0, 0, 1)},
    {Qt::Key_A, QVector3D(-1, 0, 0)},
    {Qt::Key_D, QVector3D(1, 0, 0)},
    {Qt::Key_E, QVector3D(0, 1, 0)},
    {Qt::Key_Q, QVector3D(0, -1, 0)},
};