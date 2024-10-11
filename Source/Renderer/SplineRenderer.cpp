#include "SplineRenderer.h"

#include "Core/Constants.h"
#include "Core/CurveContainer.h"

void BSplineRenderer::SplineRenderer::Initialize()
{
    initializeOpenGLFunctions();

    mSplineShader = new Shader("Spline Shader");
    mSplineShader->AddPath(QOpenGLShader::Vertex, ":/Resources/Shaders/Spline.vert");
    mSplineShader->AddPath(QOpenGLShader::TessellationControl, ":/Resources/Shaders/Spline.tcs");
    mSplineShader->AddPath(QOpenGLShader::TessellationEvaluation, ":/Resources/Shaders/Spline.tes");
    mSplineShader->AddPath(QOpenGLShader::Fragment, ":/Resources/Shaders/Spline.frag");
    mSplineShader->Initialize();
}

void BSplineRenderer::SplineRenderer::Render()
{
    if (mWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    mSplineShader->Bind();
    mSplineShader->SetUniformValue("numberOfSegments", mNumberOfSegments);
    mSplineShader->SetUniformValue("numberOfSectors", mNumberOfSectors);
    mSplineShader->SetUniformValue("VP", mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix());

    mSplineShader->SetUniformValue("light.color", mLight->GetColor());
    mSplineShader->SetUniformValue("light.direction", mLight->GetDirection());
    mSplineShader->SetUniformValue("light.ambient", mLight->GetAmbient());
    mSplineShader->SetUniformValue("light.diffuse", mLight->GetDiffuse());

    const auto& curves = mCurveContainer->GetCurves();

    for (int index = 0; index < curves.size(); ++index)
    {
        const auto& curve = curves[index];
        mSplineShader->SetUniformValue("curve.color", curve->GetColor());
        mSplineShader->SetUniformValue("curve.ambient", curve->GetAmbient());
        mSplineShader->SetUniformValue("curve.diffuse", curve->GetDiffuse());
        mSplineShader->SetUniformValue("radius", curve->GetRadius());
        curve->Render();
    }

    mSplineShader->Release();

    if (mWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void BSplineRenderer::SplineRenderer::SetCurveContainer(CurveContainer* curveContainer)
{
    mCurveContainer = curveContainer;
}

void BSplineRenderer::SplineRenderer::SetCamera(FreeCameraPtr camera)
{
    mCamera = camera;
}

void BSplineRenderer::SplineRenderer::SetLight(DirectionalLightPtr light)
{
    mLight = light;
}
