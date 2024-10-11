#pragma once

#include "Curve/Spline.h"
#include "Node/Camera/FreeCamera.h"
#include "Structs/Mouse.h"

#include <Dense>
#include <QInputEvent>
#include <QObject>

namespace BSplineRenderer
{
    class CurveContainer;
    class RendererManager;

    class EventHandler : public QObject
    {
        Q_OBJECT
      public:
        explicit EventHandler(QObject* parent);

        void OnKeyPressed(QKeyEvent*);
        void OnKeyReleased(QKeyEvent*);
        void OnMousePressed(QMouseEvent*);
        void OnMouseReleased(QMouseEvent*);
        void OnMouseMoved(QMouseEvent*);
        void OnWheelMoved(QWheelEvent*);

        void SetCamera(FreeCameraPtr camera) { mCamera = camera; }
        void SetCurveContainer(CurveContainer* curveContainer) { mCurveContainer = curveContainer; }
        void SetRendererManager(RendererManager* manager) { mRendererManager = manager; }

        Eigen::Vector3f GetViewDirection() const;

        void SetSelectedKnot(KnotPtr knot);
        void SetSelectedCurve(SplinePtr spline);

        void SetDevicePixelRatio(float devicePixelRatio) { mDevicePixelRatio = devicePixelRatio; }

      signals:
        void SelectedKnotChanged(KnotPtr knot);
        void SelectedCurveChanged(SplinePtr curve);

      private:
        void TrySelectKnot(float x, float y);
        void TrySelectCurve(float x, float y);
        void UpdateKnotTranslationPlane();
        Eigen::ParametrizedLine<float, 3> GetRayFromScreenCoordinates(float x, float y);

        FreeCameraPtr mCamera;
        Mouse mMouse;
        SplinePtr mSelectedCurve{ nullptr };
        KnotPtr mSelectedKnot{ nullptr };

        CurveContainer* mCurveContainer;
        RendererManager* mRendererManager;

        float mDevicePixelRatio{ 1.0f };

        Eigen::Hyperplane<float, 3> mKnotTranslationPlane;

        template<typename T>
        T GetCameraViewDirection()
        {
            const auto& viewDirection = mCamera->GetViewDirection();
            return T(viewDirection[0], viewDirection[1], viewDirection[2]);
        }

        template<typename T>
        T GetCameraPosition()
        {
            const auto& position = mCamera->GetPosition();

            return T(position[0], position[1], position[2]);
        }

        template<typename T>
        T GetDirectionFromScreenCoodinates(int x, int y)
        {
            const auto& direction = mCamera->GetDirectionFromScreenCoodinates(x, y);
            return T(direction[0], direction[1], direction[2]);
        }
    };

}
