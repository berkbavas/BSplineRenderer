#pragma once

#include "Camera.h"

#include <QObject>

namespace BSplineCurves3D
{
    class DummyCamera : public Camera
    {
        Q_OBJECT
    public:
        explicit DummyCamera(QObject* parent = nullptr);
        virtual ~DummyCamera();

        void KeyPressed(QKeyEvent*);
        void KeyReleased(QKeyEvent*);
        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);
        void Update(float);
    };
}
