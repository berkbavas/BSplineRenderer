#include "RendererManager.h"

#include "Core/CurveContainer.h"
#include "Renderer/SplineRenderer.h"

BSplineRenderer::RendererManager::RendererManager()
{
    mCamera = std::make_shared<FreeCamera>();
    mCamera->SetPosition(0, 0, 10);
    mCamera->MakeDirty();
    mLight = std::make_shared<DirectionalLight>();
    mLight->SetDirection(QVector3D(0, 0, 1).normalized());

    mSplineRenderer = new SplineRenderer;
    mCurveSelectionRenderer = new CurveSelectionRenderer;
}

void BSplineRenderer::RendererManager::Initialize()
{
    initializeOpenGLFunctions();

    mSplineRenderer->SetCamera(mCamera);
    mSplineRenderer->SetLight(mLight);
    mSplineRenderer->SetCurveContainer(mCurveContainer);
    mSplineRenderer->Initialize();

    mCurveSelectionRenderer->SetCamera(mCamera);
    mCurveSelectionRenderer->SetCurveContainer(mCurveContainer);
    mCurveSelectionRenderer->Initialize();

    mModelShader = new Shader("Model Shader");
    mModelShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Model.vert");
    mModelShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Model.frag");
    mModelShader->Initialize();

    mSkyBoxShader = new Shader("SkyBox Shader");
    mSkyBoxShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/SkyBox.vert");
    mSkyBoxShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/SkyBox.frag");
    mSkyBoxShader->Initialize();

    mSkyBox = std::make_shared<SkyBox>("Resources/SkyBox", ".png");

    mSphereModel = new Model(std::make_shared<Sphere>());

    Resize(INITIAL_WIDTH, INITIAL_HEIGHT);
}

void BSplineRenderer::RendererManager::Resize(int width, int height)
{
    mCurveSelectionRenderer->Resize(width, height);
}

void BSplineRenderer::RendererManager::Render()
{
    mLight->SetDirection(mCamera->GetViewDirection());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mCamera->GetWidth(), mCamera->GetHeight());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSkyBoxShader->Bind();
    mSkyBoxShader->SetUniformValue("rotation", mCamera->GetRotationMatrix());
    mSkyBoxShader->SetUniformValue("projection", mCamera->GetProjectionMatrix());
    mSkyBoxShader->SetUniformValue("skybox", 0);
    mSkyBoxShader->SetUniformValue("brightness", mSkyBox->GetBrightness());
    mSkyBox->Render();
    mSkyBoxShader->Release();

    mModelShader->Bind();
    mModelShader->SetUniformValue("viewProjectionMatrix", mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix());
    mModelShader->SetUniformValue("light.color", mLight->GetColor());
    mModelShader->SetUniformValue("light.direction", mLight->GetDirection());
    mModelShader->SetUniformValue("light.ambient", mLight->GetAmbient());
    mModelShader->SetUniformValue("light.diffuse", mLight->GetDiffuse());

    for (const auto& model : mModels)
    {
        mModelShader->SetUniformValue("modelMatrix", model->GetTransformation());
        mModelShader->SetUniformValue("normalMatrix", model->GetTransformation().normalMatrix());
        mModelShader->SetUniformValue("model.color", model->GetColor());
        mModelShader->SetUniformValue("model.ambient", model->GetAmbient());
        mModelShader->SetUniformValue("model.diffuse", model->GetDiffuse());
        model->GetMesh()->Render();
    }

    mModelShader->Release();

    if (mSelectedCurve)
    {
        RenderKnots(mSelectedCurve);
    }

    mSplineRenderer->Render();
    mCurveSelectionRenderer->Render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

BSplineRenderer::CurveQueryInfo BSplineRenderer::RendererManager::Query(const QPoint& queryPoint)
{
    return mCurveSelectionRenderer->Query(queryPoint);
}

void BSplineRenderer::RendererManager::AddModel(ModelPtr model)
{
    mModels << model;
}

void BSplineRenderer::RendererManager::RemoveModel(ModelPtr model)
{
    mModels.removeAll(model);
}

void BSplineRenderer::RendererManager::SetNumberOfSegments(int numberOfSegments)
{
    mSplineRenderer->SetNumberOfSegments(numberOfSegments);
    mCurveSelectionRenderer->SetNumberOfSegments(numberOfSegments);
}

void BSplineRenderer::RendererManager::SetNumberOfSectors(int numberOfSectors)
{
    mSplineRenderer->SetNumberOfSectors(numberOfSectors);
    mCurveSelectionRenderer->SetNumberOfSectors(numberOfSectors);
}

int BSplineRenderer::RendererManager::GetNumberOfSegments() const
{
    return mSplineRenderer->GetNumberOfSegments();
}

int BSplineRenderer::RendererManager::GetNumberOfSectors() const
{
    return mSplineRenderer->GetNumberOfSectors();
}

bool* BSplineRenderer::RendererManager::GetWireframe()
{
    return &mSplineRenderer->GetWireframe_NonConst();
}

void BSplineRenderer::RendererManager::RenderKnots(SplinePtr curve)
{
    const auto& knots = curve->GetKnots();
    const float r = curve->GetRadius();

    mModelShader->Bind();
    mModelShader->SetUniformValue("model.ambient", mSphereModel->GetAmbient());
    mModelShader->SetUniformValue("model.diffuse", mSphereModel->GetDiffuse());

    for (const auto& knot : knots)
    {
        if (mSelectedKnot == knot)
        {
            mSphereModel->SetColor(QVector4D(0, 1, 0, 1));
            mSphereModel->SetScale(2 * r, 2 * r, 2 * r);
        }
        else
        {
            mSphereModel->SetColor(QVector4D(1, 1, 0, 1));
            mSphereModel->SetScale(2 * r, 2 * r, 2 * r);
        }

        mSphereModel->SetPosition(knot->GetPosition());
        mModelShader->SetUniformValue("modelMatrix", mSphereModel->GetTransformation());
        mModelShader->SetUniformValue("normalMatrix", mSphereModel->GetTransformation().normalMatrix());
        mModelShader->SetUniformValue("model.color", mSphereModel->GetColor());

        mSphereModel->GetMesh()->Render();
    }

    mModelShader->Release();
}
