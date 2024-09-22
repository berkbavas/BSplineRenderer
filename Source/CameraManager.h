#pragma once

#include "Camera.h"

#include <QObject>

namespace BSplineCurves3D
{
    class CameraManager : public QObject
    {
        Q_OBJECT

    private:
        explicit CameraManager(QObject* parent = nullptr);

    public:
        void AddCamera(Camera* camera);
        void RemoveCamera(Camera* camera);

        void KeyPressed(QKeyEvent* event);
        void KeyReleased(QKeyEvent* event);
        void MousePressed(QMouseEvent* event);
        void MouseReleased(QMouseEvent* event);
        void MouseMoved(QMouseEvent* event);
        void Update(float ifps);

        QVector3D GetViewDirection() const;
        QVector3D GetDirectionFromScreen(int x, int y, int width, int height) const;

        Camera* GetActiveCamera() const;
        void SetActiveCamera(Camera* newActiveCamera);

        static CameraManager* Instance();

    private:
        Camera* mActiveCamera;
        QList<Camera*> mCameras;
    };
}
