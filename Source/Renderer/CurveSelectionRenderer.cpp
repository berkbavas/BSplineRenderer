#include "CurveSelectionRenderer.h"

void BSplineRenderer::CurveSelectionRenderer::Initialize()
{
    initializeOpenGLFunctions();

    mShader = new Shader("Curve Selection Shader");
    mShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Spline.vert");
    mShader->AddPath(QOpenGLShader::TessellationControl, ":/Resources/Shaders/Spline.tcs");
    mShader->AddPath(QOpenGLShader::TessellationEvaluation, ":/Resources/Shaders/Spline.tes");
    mShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/CurveSelection.frag");
    mShader->Initialize();
}

void BSplineRenderer::CurveSelectionRenderer::Render()
{
    mFramebuffer->Clear();
    mFramebuffer->Bind();

    mShader->Bind();
    mShader->SetUniformValue("numberOfSegments", mNumberSegments);
    mShader->SetUniformValue("numberOfSectors", mNumberSegments);
    mShader->SetUniformValue("VP", mCamera->GetViewProjectionMatrix());

    const auto& curves = mCurveContainer->GetCurves();

    for (int index = 0; index < curves.size(); ++index)
    {
        const auto& curve = curves[index];
        mShader->SetUniformValue("curveIndex", index);
        mShader->SetUniformValue("radius", curve->GetRadius());
        curve->Render();
    }

    mShader->Release();
}

void BSplineRenderer::CurveSelectionRenderer::Resize(int width, int height)
{
    if (mFramebuffer)
    {
        delete mFramebuffer;
        mFramebuffer = nullptr;
    }

    mFramebuffer = new CurveSelectionFramebuffer(width, height);
}

BSplineRenderer::CurveQueryInfo BSplineRenderer::CurveSelectionRenderer::Query(const QPoint& queryPoint)
{
    return mFramebuffer->Query(queryPoint);
}

void BSplineRenderer::CurveSelectionRenderer::SetCurveContainer(CurveContainer* curveContainer)
{
    mCurveContainer = curveContainer;
}

void BSplineRenderer::CurveSelectionRenderer::SetCamera(FreeCameraPtr camera)
{
    mCamera = camera;
}
