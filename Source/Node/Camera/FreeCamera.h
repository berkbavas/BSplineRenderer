#pragma once

#include "Node/Node.h"
#include "Structs/Mouse.h"

#include <QMap>
#include <QMouseEvent>

namespace BSplineRenderer
{
    class FreeCamera : public Node
    {
      public:
        FreeCamera() = default;

        void Update(float ifps);
        void Reset();
        void Resize(int w, int h);

        void KeyPressed(QKeyEvent*);
        void KeyReleased(QKeyEvent*);
        void MousePressed(QMouseEvent*);
        void MouseReleased(QMouseEvent*);
        void MouseMoved(QMouseEvent*);

        QVector3D GetDirectionFromScreenCoodinates(int x, int y);

        const QMatrix4x4& GetProjectionMatrix();
        const QMatrix4x4& GetViewProjectionMatrix();
        const QMatrix4x4& GetRotationMatrix();
        const QMatrix4x4& GetViewMatrix();
        const QVector3D& GetViewDirection();

        float GetHorizontalFov() const;
        float GetAspectRatio() const;

      private:
        DEFINE_MEMBER(int, Width, 1600);
        DEFINE_MEMBER(int, Height, 900);
        DEFINE_MEMBER(float, ZNear, 0.1f);
        DEFINE_MEMBER(float, ZFar, 10000.0f);
        DEFINE_MEMBER(float, VerticalFov, 60.0f);

        DEFINE_MEMBER(float, AngularSpeed, 25.0f);
        DEFINE_MEMBER(float, LinearSpeed, 5.0f);
        DEFINE_MEMBER(float, LinearSpeedMultiplier, 1.0f);
        DEFINE_MEMBER(float, AngularSpeedMultiplier, 1.0f);
        DEFINE_MEMBER(Qt::MouseButton, ActionReceiveButton, Qt::MiddleButton)

        QMap<Qt::Key, bool> mPressedKeys;

        Mouse mMouse;

        // For caching
        QMatrix4x4 mViewProjectionMatrix;
        QMatrix4x4 mProjectionMatrix;
        QMatrix4x4 mViewMatrix;
        QMatrix4x4 mRotationMatrix;
        QVector3D mViewDirection;

        bool mUpdateRotation{ false };
        bool mUpdatePosition{ false };

        static const QMap<Qt::Key, QVector3D> KEY_BINDINGS;
    };

    using FreeCameraPtr = std::shared_ptr<FreeCamera>;
}