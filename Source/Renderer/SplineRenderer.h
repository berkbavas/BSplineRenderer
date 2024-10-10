#pragma once

#include "Node/Camera/FreeCamera.h"
#include "Node/Light/DirectionalLight.h"
#include "Node/Mesh/Sphere.h"
#include "Node/Model/Model.h"
#include "Renderer/Base/Shader.h"

#include <QOpenGLFunctions_4_5_Core>

namespace BSplineRenderer
{

    class CurveContainer;

    class SplineRenderer : protected QOpenGLFunctions_4_5_Core
    {
      public:
        SplineRenderer() = default;

        void Initialize();
        void Render();
        void SetCurveContainer(CurveContainer* CurveContainer);
        void SetCamera(FreeCameraPtr camera);
        void SetLight(DirectionalLightPtr light);

      private:
        CurveContainer* mCurveContainer;
        FreeCameraPtr mCamera;
        DirectionalLightPtr mLight;

        Shader* mSplineShader;

        DEFINE_MEMBER(bool, Wireframe, false);
        DEFINE_MEMBER(int, NumberSegments, DEFAULT_NUMBER_OF_SEGMENTS);
        DEFINE_MEMBER(int, NumberOfSectors, DEFAULT_NUMBER_OF_SECTORS);
    };
}