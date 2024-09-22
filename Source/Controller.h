#pragma once

#include "Camera.h"
#include "Enums.h"
#include "RendererManager.h"

#include <QObject>

#include <Dense>
#include <QFileDialog>

namespace BSplineCurves3D
{
    class Window;

    class Controller : public QObject
    {
        Q_OBJECT
    public:
        explicit Controller(QObject* parent = nullptr);

        void Init();
        void WheelMoved(QWheelEvent* event);
        void MousePressed(QMouseEvent* event);
        void MouseReleased(QMouseEvent* event);
        void MouseMoved(QMouseEvent* event);
        void KeyPressed(QKeyEvent* event);
        void KeyReleased(QKeyEvent* event);
        void Resize(int w, int h);
        void MouseDoubleClicked(QMouseEvent* event);
        void Render(float ifps);
        void OnAction(Action, QVariant variant = QVariant());

        void SetWindow(Window* newWindow);

    signals:
        void ModeChanged(Mode newMode);

    private:
        RendererManager* mRendererManager;
        CameraManager* mCameraManager;
        LightManager* mLightManager;
        ModelManager* mModelManager;
        CurveManager* mCurveManager;
        Window* mWindow;

        Camera* mCamera;
        Light* mLight;

        Model* mPlane;

        Spline* mSelectedCurve;
        KnotPoint* mSelectedKnotPoint;

        Qt::MouseButton mPressedButton;
        Eigen::Hyperplane<float, 3> mTranslationPlane;

        Mode mMode;

        QFileDialog* mFileDialog;
        Action mLastFileAction;
    };
}
