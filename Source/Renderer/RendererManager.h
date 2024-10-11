#pragma once

#include "Curve/Spline.h"
#include "Node/Camera/FreeCamera.h"
#include "Node/Light/DirectionalLight.h"
#include "Node/Mesh/Plane.h"
#include "Node/Mesh/Sphere.h"
#include "Node/Model/Model.h"
#include "Node/SkyBox/SkyBox.h"
#include "Renderer/Base/Shader.h"
#include "Renderer/CurveSelectionRenderer.h"

#include <QMap>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector>

namespace BSplineRenderer
{

    class CurveContainer;
    class SplineRenderer;

    class RendererManager : protected QOpenGLFunctions_4_5_Core
    {
      public:
        RendererManager();

        void Initialize();
        void Resize(int width, int height);
        void Render();

        CurveQueryInfo Query(const QPoint& queryPoint);

        void AddModel(ModelPtr model);
        void RemoveModel(ModelPtr model);
        FreeCameraPtr GetCamera() const { return mCamera; }

        void SetCurveContainer(CurveContainer* curveContainer) { mCurveContainer = curveContainer; }

        void SetNumberOfSegments(int numberOfSegments);
        void SetNumberOfSectors(int numberOfSectors);

        int GetNumberOfSegments() const;
        int GetNumberOfSectors() const;

        bool* GetWireframe();

      public slots:
        void SetSelectedCurve(SplinePtr spline) { mSelectedCurve = spline; }
        void SetSelectedKnot(KnotPtr knot) { mSelectedKnot = knot; }

      private:
        void RenderKnots(SplinePtr curve);

        Shader* mModelShader;
        Shader* mSkyBoxShader;
        CurveContainer* mCurveContainer;
        FreeCameraPtr mCamera;
        DirectionalLightPtr mLight;

        QVector<ModelPtr> mModels;

        SkyBoxPtr mSkyBox;
        Model* mSphereModel;

        SplineRenderer* mSplineRenderer;
        CurveSelectionRenderer* mCurveSelectionRenderer;

        SplinePtr mSelectedCurve{ nullptr };
        KnotPtr mSelectedKnot{ nullptr };
    };
}