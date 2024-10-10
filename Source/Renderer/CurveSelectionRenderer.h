#pragma once

#include "Core/CurveContainer.h"
#include "Node/Camera/FreeCamera.h"
#include "Renderer/Base/CurveSelectionFramebuffer.h"
#include "Renderer/Base/Shader.h"

#include <QOpenGLExtraFunctions>

namespace BSplineRenderer
{
    class CurveSelectionRenderer : protected QOpenGLExtraFunctions
    {
      public:
        CurveSelectionRenderer() = default;

        void Initialize();
        void Render();
        void Resize(int width, int height);

        CurveQueryInfo Query(const QPoint& queryPoint);

        void SetCurveContainer(CurveContainer* curveContainer);
        void SetCamera(FreeCameraPtr camera);

      private:
        CurveContainer* mCurveContainer;
        Shader* mShader;
        FreeCameraPtr mCamera;
        CurveSelectionFramebuffer* mFramebuffer{ nullptr };

        DEFINE_MEMBER(int, NumberSegments, DEFAULT_NUMBER_OF_SEGMENTS);
        DEFINE_MEMBER(int, NumberOfSectors, DEFAULT_NUMBER_OF_SECTORS);
    };
}