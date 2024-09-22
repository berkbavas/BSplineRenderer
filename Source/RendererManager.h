#pragma once

#include "CameraManager.h"
#include "CurveManager.h"
#include "Light.h"
#include "LightManager.h"
#include "ModelData.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Ticks.h"

#include <QMap>
#include <QMatrix4x4>
#include <QObject>

namespace BSplineCurves3D
{
    class RendererManager : public QObject, protected QOpenGLFunctions
    {
    private:
        explicit RendererManager(QObject* parent = nullptr);

    public:
        static RendererManager* Instance();

        bool Init();
        void Render(float ifps);

        void SetRenderPaths(bool newRenderPaths);
        void SetRenderPipes(bool newRenderPipes);

        bool GetRenderPaths() const;
        bool GetRenderPipes() const;

    private slots:
        void RenderModels(float ifps);
        void RenderKnotPoints(float ifps);
        void RenderPaths(float ifps);
        void RenderPipes(float ifps);

        void RenderUsingDumbShader(float ifps, Spline* curve, Bezier* patch);
        void RenderUsingSmartShader(float ifps, Spline* curve, Bezier* patch);

    private:
        QMap<Model::Type, ModelData*> mTypeToModelData;

        Ticks* mPathTicks;
        Ticks* mPipeTicks;

        ModelManager* mModelManager;
        CameraManager* mCameraManager;
        LightManager* mLightManager;
        CurveManager* mCurveManager;
        ShaderManager* mShaderManager;

        Spline* mSelectedCurve;
        KnotPoint* mSelectedKnotPoint;

        Camera* mCamera;
        Light* mLight;

        Model* mKnotPointModel;
        ModelData* mKnotPointModelData;

        bool mRenderPaths;
        bool mRenderPipes;
    };
}
