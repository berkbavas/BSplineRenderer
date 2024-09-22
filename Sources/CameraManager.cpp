#include "CameraManager.h"
#include <QtMath>

BSplineCurves3D::CameraManager::CameraManager(QObject* parent)
    : QObject(parent)
    , mActiveCamera(nullptr)
{}

void BSplineCurves3D::CameraManager::AddCamera(Camera* camera)
{
    mCameras << camera;
}

void BSplineCurves3D::CameraManager::RemoveCamera(Camera* camera)
{
    if (camera)
    {
        if (camera == mActiveCamera)
            mActiveCamera = nullptr;

        mCameras.removeAll(camera);
        camera->deleteLater();
    }
}

BSplineCurves3D::Camera* BSplineCurves3D::CameraManager::GetActiveCamera() const
{
    return mActiveCamera;
}

void BSplineCurves3D::CameraManager::SetActiveCamera(Camera* newActiveCamera)
{
    mActiveCamera = newActiveCamera;
}

void BSplineCurves3D::CameraManager::KeyPressed(QKeyEvent* event)
{
    if (mActiveCamera)
        mActiveCamera->KeyPressed(event);
}

void BSplineCurves3D::CameraManager::KeyReleased(QKeyEvent* event)
{
    if (mActiveCamera)
        mActiveCamera->KeyReleased(event);
}

void BSplineCurves3D::CameraManager::MousePressed(QMouseEvent* event)
{
    if (mActiveCamera)
        mActiveCamera->MousePressed(event);
}

void BSplineCurves3D::CameraManager::MouseReleased(QMouseEvent* event)
{
    if (mActiveCamera)
        mActiveCamera->MouseReleased(event);
}

void BSplineCurves3D::CameraManager::MouseMoved(QMouseEvent* event)
{
    if (mActiveCamera)
        mActiveCamera->MouseMoved(event);
}

void BSplineCurves3D::CameraManager::Update(float ifps)
{
    if (mActiveCamera)
        mActiveCamera->Update(ifps);
}

QVector3D BSplineCurves3D::CameraManager::GetViewDirection() const
{
    if (mActiveCamera)
        return mActiveCamera->GetViewDirection();

    return QVector3D();
}

QVector3D BSplineCurves3D::CameraManager::GetDirectionFromScreen(int x, int y, int width, int height) const
{
    if (mActiveCamera)
    {
        const float halfVerticalFovRadian = 0.5f * qDegreesToRadians(mActiveCamera->GetVerticalFov());
        const float halfHorizontalFovRadian = atan(tan(halfVerticalFovRadian) * mActiveCamera->GetAspectRatio());

        const float horizontalRotationAngleRadian = atan(tan(halfHorizontalFovRadian) * (0.5f * width - x) / (0.5f * width));
        const float horizontalRotationAngle = qRadiansToDegrees(horizontalRotationAngleRadian);

        const float verticalRotationAngleRadian = atan(((0.5f * height - y) / (0.5f * width - x)) * sin(horizontalRotationAngleRadian));
        const float verticalRotationAngle = qRadiansToDegrees(verticalRotationAngleRadian);

        QQuaternion left = mActiveCamera->Rotation() * QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), horizontalRotationAngle);
        QQuaternion leftThenUp = left * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), verticalRotationAngle);

        return leftThenUp * QVector3D(0, 0, -1);
    }

    return QVector3D();
}

BSplineCurves3D::CameraManager* BSplineCurves3D::CameraManager::Instance()
{
    static CameraManager instance;
    return &instance;
}