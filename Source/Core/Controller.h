#pragma once

#include "Core/Constants.h"
#include "EventHandler/EventHandler.h"
#include "Util/Macros.h"

#include <QMouseEvent>
#include <QObject>
#include <QOpenGLExtraFunctions>

namespace BSplineRenderer
{
    class Window;
    class ImGuiWindow;
    class RendererManager;
    class CurveContainer;

    class Controller : public QObject, protected QOpenGLExtraFunctions
    {
        DISABLE_COPY(Controller);

        Q_OBJECT
      public:
        explicit Controller(QObject* parent = nullptr);
        ~Controller();

        void Run();

      public slots:
        // Core Events
        void Initialize();
        void Resize(int w, int h);
        void Render(float ifps);

        // Input Events
        void OnKeyPressed(QKeyEvent*);
        void OnKeyReleased(QKeyEvent*);
        void OnMousePressed(QMouseEvent*);
        void OnMouseReleased(QMouseEvent*);
        void OnMouseMoved(QMouseEvent*);
        void OnWheelMoved(QWheelEvent*);

      private:
        void ApplyCameraPreset(int preset);

        float mDevicePixelRatio{ 1.0f };
        float mWidth{ INITIAL_WIDTH };
        float mHeight{ INITIAL_HEIGHT };

        Window* mWindow;
        ImGuiWindow* mImGuiWindow;

        EventHandler* mEventHandler;
        RendererManager* mRendererManager;
        CurveContainer* mCurveContainer;
        FreeCameraPtr mCamera;
    };
}
