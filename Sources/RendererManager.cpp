#include "RendererManager.h"
#include "Bezier.h"
#include "Camera.h"
#include "Light.h"

#include <QtMath>

BSplineCurves3D::RendererManager::RendererManager(QObject* parent)
    : QObject(parent)
    , mSelectedCurve(nullptr)
    , mSelectedKnotPoint(nullptr)
    , mRenderPaths(true)
    , mRenderPipes(true)
{
    mModelManager = ModelManager::Instance();
    mCameraManager = CameraManager::Instance();
    mLightManager = LightManager::Instance();
    mCurveManager = CurveManager::Instance();
    mShaderManager = ShaderManager::Instance();

    connect(mCurveManager, &CurveManager::SelectedCurveChanged, this, [=](Spline* selectedCurve) { mSelectedCurve = selectedCurve; });
    connect(mCurveManager, &CurveManager::SelectedKnotPointChanged, this, [=](KnotPoint* selectedPoint) { mSelectedKnotPoint = selectedPoint; });
}

BSplineCurves3D::RendererManager* BSplineCurves3D::RendererManager::Instance()
{
    static RendererManager instance;
    return &instance;
}

bool BSplineCurves3D::RendererManager::Init()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLineWidth(2.5f);

    qInfo() << Q_FUNC_INFO << "Initializing ShaderManager...";

    if (!mShaderManager->Init())
    {
        qWarning() << Q_FUNC_INFO << "ShaderManager could not be initialized.";
        return false;
    }

    qInfo() << Q_FUNC_INFO << "Loading and creating all models...";

    for (Model::Type type : Model::ALL_MODEL_TYPES)
    {
        ModelData* data = new ModelData(type);
        if (!data->Load() || !data->Create())
        {
            data->deleteLater();
            continue;
        }

        mTypeToModelData.insert(type, data);
    }

    // Knot Point Model
    {
        mKnotPointModel = new Model;
        mKnotPointModel->SetType(Model::Sphere);
        mKnotPointModel->setObjectName("KnotPointModel");
        mKnotPointModel->GetMaterial_NonConst().SetColor(QVector4D(0, 1, 0, 1));
        mKnotPointModel->SetScale(QVector3D(0.005f, 0.005f, 0.005f));
        mKnotPointModel->SetVisible(false);

        mKnotPointModelData = mTypeToModelData.value(mKnotPointModel->GetType(), nullptr);
    }

    mPathTicks = new Ticks(0, 1, 100);
    mPathTicks->Create();

    mPipeTicks = new Ticks(0, 1, 100);
    mPipeTicks->Create();

    return true;
}

void BSplineCurves3D::RendererManager::Render(float ifps)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCamera = mCameraManager->GetActiveCamera();
    mLight = mLightManager->GetActiveLight();

    RenderModels(ifps);

    if (mRenderPaths)
        RenderPaths(ifps);

    if (mRenderPipes)
        RenderPipes(ifps);

    if (mRenderPaths || mRenderPipes)
        RenderKnotPoints(ifps);
}

void BSplineCurves3D::RendererManager::RenderModels(float ifps)
{
    Q_UNUSED(ifps);

    mShaderManager->Bind(ShaderManager::Shader::Basic);

    if (mCamera)
    {
        mShaderManager->SetUniformValue("projection_matrix", mCamera->GetProjectionMatrix());
        mShaderManager->SetUniformValue("view_matrix", mCamera->GetViewMatrix());
        mShaderManager->SetUniformValue("camera_position", mCamera->Position());
    }

    if (mLight)
    {
        mShaderManager->SetUniformValue("light.position", mLight->Position());
        mShaderManager->SetUniformValue("light.color", mLight->GetColor());
        mShaderManager->SetUniformValue("light.ambient", mLight->GetAmbient());
        mShaderManager->SetUniformValue("light.diffuse", mLight->GetDiffuse());
        mShaderManager->SetUniformValue("light.specular", mLight->GetSpecular());
    }

    QList<Model*> models = mModelManager->GetModels();

    for (Model* model : qAsConst(models))
    {
        if (!model->GetVisible())
            continue;

        ModelData* data = mTypeToModelData.value(model->GetType(), nullptr);

        if (data)
        {
            mShaderManager->SetUniformValue("node.transformation", model->Transformation());
            mShaderManager->SetUniformValue("node.color", model->GetMaterial().GetColor());
            mShaderManager->SetUniformValue("node.ambient", model->GetMaterial().GetAmbient());
            mShaderManager->SetUniformValue("node.diffuse", model->GetMaterial().GetDiffuse());
            mShaderManager->SetUniformValue("node.specular", model->GetMaterial().GetSpecular());
            mShaderManager->SetUniformValue("node.shininess", model->GetMaterial().GetShininess());
            data->Render();
        }
    }

    mShaderManager->Release();
}

void BSplineCurves3D::RendererManager::RenderKnotPoints(float ifps)
{
    Q_UNUSED(ifps);

    mShaderManager->Bind(ShaderManager::Shader::Basic);

    if (mCamera)
    {
        mShaderManager->SetUniformValue("projection_matrix", mCamera->GetProjectionMatrix());
        mShaderManager->SetUniformValue("view_matrix", mCamera->GetViewMatrix());
        mShaderManager->SetUniformValue("camera_position", mCamera->Position());
    }

    if (mLight)
    {
        mShaderManager->SetUniformValue("light.position", mLight->Position());
        mShaderManager->SetUniformValue("light.color", mLight->GetColor());
        mShaderManager->SetUniformValue("light.ambient", mLight->GetAmbient());
        mShaderManager->SetUniformValue("light.diffuse", mLight->GetDiffuse());
        mShaderManager->SetUniformValue("light.specular", mLight->GetSpecular());
    }

    if (mSelectedCurve)
    {
        mShaderManager->SetUniformValue("node.ambient", mKnotPointModel->GetMaterial().GetAmbient());
        mShaderManager->SetUniformValue("node.diffuse", mKnotPointModel->GetMaterial().GetDiffuse());
        mShaderManager->SetUniformValue("node.specular", mKnotPointModel->GetMaterial().GetSpecular());
        mShaderManager->SetUniformValue("node.shininess", mKnotPointModel->GetMaterial().GetShininess());

        for (auto& point : qAsConst(mSelectedCurve->GetKnotPoints()))
        {
            mKnotPointModel->SetPosition(point->GetPosition());
            mKnotPointModel->GetMaterial_NonConst().SetColor(point->GetSelected() ? QVector4D(1, 1, 0, 1) : QVector4D(0, 1, 0, 1));

            mShaderManager->SetUniformValue("node.color", mKnotPointModel->GetMaterial().GetColor());
            mShaderManager->SetUniformValue("node.transformation", mKnotPointModel->Transformation());

            mKnotPointModelData->Render();
        }
    }

    mShaderManager->Release();
}

void BSplineCurves3D::RendererManager::RenderPaths(float ifps)
{
    Q_UNUSED(ifps);

    mShaderManager->Bind(ShaderManager::Shader::Path);

    if (mCamera)
    {
        mShaderManager->SetUniformValue("projection_matrix", mCamera->GetProjectionMatrix());
        mShaderManager->SetUniformValue("view_matrix", mCamera->GetViewMatrix());
    }

    mShaderManager->SetUniformValue("color", QVector4D(1, 0, 0, 1));

    for (auto& curve : qAsConst(mCurveManager->GetCurves()))
    {
        if (curve)
        {
            QList<Bezier*> patches = curve->GetBezierPatches();
            for (auto& patch : qAsConst(patches))
            {
                auto controlPointPositions = patch->GetControlPointPositions();

                mShaderManager->SetUniformValue("control_points_count", static_cast<int>(controlPointPositions.size()));
                mShaderManager->SetUniformValueArray("control_points", controlPointPositions);
                mPathTicks->Render();
            }
        }
    }

    mShaderManager->Release();
}

void BSplineCurves3D::RendererManager::RenderPipes(float ifps)
{
    // Pipe

    for (auto& curve : qAsConst(mCurveManager->GetCurves()))
    {
        if (curve)
        {
            QList<Bezier*> patches = curve->GetBezierPatches();

            for (auto& patch : patches)
            {
                if (!patch->GetInitialized())
                {
                    patch->InitializeOpenGLStuff();
                }
                
                if (patch->GetVertexGenerationStatus() == Bezier::VertexGenerationStatus::GeneratingVertices)
                {
                    RenderUsingDumbShader(ifps, curve, patch);
                }
                else if (patch->GetVertexGenerationStatus() == Bezier::VertexGenerationStatus::WaitingForOpenGLUpdate)
                {
                    patch->UpdateOpenGLStuff();
                    RenderUsingSmartShader(ifps, curve, patch);
                }
                else if (patch->GetVertexGenerationStatus() == Bezier::VertexGenerationStatus::Dirty)
                {
                    patch->GenerateVertices();
                    RenderUsingDumbShader(ifps, curve, patch);
                }
                else if (patch->GetVertexGenerationStatus() == Bezier::VertexGenerationStatus::Ready)
                {
                    RenderUsingSmartShader(ifps, curve, patch);
                }
            }
        }
    }
}

void BSplineCurves3D::RendererManager::RenderUsingDumbShader(float ifps, Spline* curve, Bezier* patch)
{
    Q_UNUSED(ifps);

    mShaderManager->Bind(ShaderManager::Shader::PipeDumb);

    if (mCamera)
    {
        mShaderManager->SetUniformValue("projection_matrix", mCamera->GetProjectionMatrix());
        mShaderManager->SetUniformValue("view_matrix", mCamera->GetViewMatrix());
        mShaderManager->SetUniformValue("camera_position", mCamera->Position());
    }

    if (mLight)
    {
        mShaderManager->SetUniformValue("light.position", mLight->Position());
        mShaderManager->SetUniformValue("light.color", mLight->GetColor());
        mShaderManager->SetUniformValue("light.ambient", mLight->GetAmbient());
        mShaderManager->SetUniformValue("light.diffuse", mLight->GetDiffuse());
        mShaderManager->SetUniformValue("light.specular", mLight->GetSpecular());
    }

    mShaderManager->SetUniformValue("dt", mPipeTicks->GetTicksDelta());

    auto controlPointPositions = patch->GetControlPointPositions();

    mShaderManager->SetUniformValue("control_points_count", static_cast<int>(controlPointPositions.size()));
    mShaderManager->SetUniformValueArray("control_points", controlPointPositions);

    mShaderManager->SetUniformValue("node.color", curve->GetMaterial().GetColor());
    mShaderManager->SetUniformValue("node.ambient", curve->GetMaterial().GetAmbient());
    mShaderManager->SetUniformValue("node.diffuse", curve->GetMaterial().GetDiffuse());
    mShaderManager->SetUniformValue("node.specular", curve->GetMaterial().GetSpecular());
    mShaderManager->SetUniformValue("node.shininess", curve->GetMaterial().GetShininess());

    int n = patch->GetSectorCount();
    float r = patch->GetRadius();

    for (int i = 0; i < n; i++)
    {
        float sectorAngle0 = 2.0f * float(i) * M_PI / n;
        float sectorAngle1 = 2.0f * float(i + 1) * M_PI / n;

        mShaderManager->SetUniformValue("r", r);
        mShaderManager->SetUniformValue("sector_angle_0", sectorAngle0);
        mShaderManager->SetUniformValue("sector_angle_1", sectorAngle1);
        mPipeTicks->Render();
    }

    mShaderManager->Release();
}

void BSplineCurves3D::RendererManager::RenderUsingSmartShader(float ifps, Spline* curve, Bezier* patch)
{
    Q_UNUSED(ifps);

    mShaderManager->Bind(ShaderManager::Shader::PipeSmart);

    if (mCamera)
    {
        mShaderManager->SetUniformValue("projection_matrix", mCamera->GetProjectionMatrix());
        mShaderManager->SetUniformValue("view_matrix", mCamera->GetViewMatrix());
        mShaderManager->SetUniformValue("camera_position", mCamera->Position());
    }

    if (mLight)
        Q_UNUSED(ifps);
    {
        mShaderManager->SetUniformValue("light.position", mLight->Position());
        mShaderManager->SetUniformValue("light.color", mLight->GetColor());
        mShaderManager->SetUniformValue("light.ambient", mLight->GetAmbient());
        mShaderManager->SetUniformValue("light.diffuse", mLight->GetDiffuse());
        mShaderManager->SetUniformValue("light.specular", mLight->GetSpecular());
    }

    mShaderManager->SetUniformValue("node.color", curve->GetMaterial().GetColor());
    mShaderManager->SetUniformValue("node.ambient", curve->GetMaterial().GetAmbient());
    mShaderManager->SetUniformValue("node.diffuse", curve->GetMaterial().GetDiffuse());
    mShaderManager->SetUniformValue("node.specular", curve->GetMaterial().GetSpecular());
    mShaderManager->SetUniformValue("node.shininess", curve->GetMaterial().GetShininess());

    patch->Render();

    mShaderManager->Release();
}

void BSplineCurves3D::RendererManager::SetRenderPipes(bool newRenderPipes)
{
    mRenderPipes = newRenderPipes;
}

void BSplineCurves3D::RendererManager::SetRenderPaths(bool newRenderPaths)
{
    mRenderPaths = newRenderPaths;
}

bool BSplineCurves3D::RendererManager::GetRenderPaths() const
{
    return mRenderPaths;
}

bool BSplineCurves3D::RendererManager::GetRenderPipes() const
{
    return mRenderPipes;
}