#pragma once

#include "Camera.h"

#include <QObject>

namespace BSplineCurves3D
{
    class FreeCamera : public Camera
    {
        Q_OBJECT

    public:
        explicit FreeCamera(QObject* parent = nullptr);

        void KeyPressed(QKeyEvent* event) override;
        void KeyReleased(QKeyEvent* event) override;
        void MousePressed(QMouseEvent* event) override;
        void MouseReleased(QMouseEvent* event) override;
        void MouseMoved(QMouseEvent* event) override;
        void Update(float ifps) override;

    private:
        QMap<Qt::Key, bool> mPressedKeys;

        float mMovementSpeed;
        float mAngularSpeed;

        bool mMousePressed;
        float mMousePreviousX;
        float mMousePreviousY;
        float mMouseDeltaX;
        float mMouseDeltaY;

        bool mUpdateRotation;
        bool mUpdatePosition;

        static const QMap<Qt::Key, QVector3D> KEY_BINDINGS;
    };
}
