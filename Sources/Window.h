#pragma once

#include "Enums.h"
#include "Light.h"
#include "LightManager.h"
#include "RendererManager.h"

#include <QOpenGLFunctionsPrivate>
#include <QOpenGLWindow>

namespace BSplineCurves3D
{
    class Controller;

    class Window : public QOpenGLWindow, protected QOpenGLFunctions
    {
        Q_OBJECT

    public:
        Window(QWindow* parent = nullptr);

    public slots:
        void OnModeChanged(Mode newMode);

    private:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void keyPressEvent(QKeyEvent*) override;
        void keyReleaseEvent(QKeyEvent*) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void wheelEvent(QWheelEvent*) override;
        void mouseDoubleClickEvent(QMouseEvent*) override;

    private:
        Controller* mController;
        long long mPreviousTime;
        long long mCurrentTime;

        bool mImguiWantCapture;

        RendererManager* mRendererManager;
        CurveManager* mCurveManager;
        LightManager* mLightManager;

        Light* mActiveLight;

        Mode mMode;

        bool mRenderPaths;
        bool mRenderPipes;

        Spline* mSelectedCurve;
        KnotPoint* mSelectedKnotPoint;

        float mGlobalPipeRadius;
        int mGlobalPipeSectorCount;
    };
}
