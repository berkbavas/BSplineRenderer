#pragma once

#include "Node.h"
#include "Macros.h"

#include <QMatrix4x4>
#include <QMouseEvent>

namespace BSplineCurves3D
{
    class Camera : public Node
    {
        Q_OBJECT
    protected:
        explicit Camera(QObject* parent = nullptr);

    public:
        virtual float GetVerticalFov() const;
        virtual float GetHorizontalFov() const;

        virtual void SetVerticalFov(float newVerticalFov);
        virtual void SetHorizontalFov(float newHorizontalFov);

        virtual void KeyPressed(QKeyEvent*) = 0;
        virtual void KeyReleased(QKeyEvent*) = 0;
        virtual void MousePressed(QMouseEvent*) = 0;
        virtual void MouseReleased(QMouseEvent*) = 0;
        virtual void MouseMoved(QMouseEvent*) = 0;
        virtual void Update(float) = 0;

        QMatrix4x4 GetViewMatrix() const;
        QMatrix4x4 GetProjectionMatrix() const;
        QVector3D GetViewDirection() const;

    protected:
        float mVerticalFov;
        float mHorizontalFov;

        DEFINE_MEMBER(float, AspectRatio);
        DEFINE_MEMBER(float, ZNear);
        DEFINE_MEMBER(float, ZFar);
    };
}
