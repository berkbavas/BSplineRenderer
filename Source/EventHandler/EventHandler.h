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
        void TrySelectCurve(const QPoint& mousePosition);

        FreeCameraPtr mCamera;
        Mouse mMouse;
        SplinePtr mSelectedCurve{ nullptr };
        KnotPtr mSelectedKnot{ nullptr };

        CurveContainer* mCurveContainer;
        RendererManager* mRendererManager;

        float mDevicePixelRatio{ 1.0f };
    };

}
