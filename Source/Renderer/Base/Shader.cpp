#include "Shader.h"

#include "Util/Util.h"

#include <QDebug>
#include <QFile>

BSplineRenderer::Shader::Shader(const QString& name)
    : mProgram(nullptr)
    , mName(name)
{
}

void BSplineRenderer::Shader::Initialize()
{
    LOG_INFO("Shader::Initialize: '{}' is being initializing.", mName.toStdString());

    initializeOpenGLFunctions();

    mProgram = QSharedPointer<QOpenGLShaderProgram>(new QOpenGLShaderProgram);

    for (const auto [shaderType, path] : mPaths)
    {
        const auto bytes = Util::GetBytes(path);
        if (!mProgram->addShaderFromSourceCode(shaderType, bytes))
        {
            BR_EXIT_FAILURE("Shader::Initialize: '{}' could not be loaded.", GetShaderTypeString(shaderType).toStdString());
        }
    }

    if (!mProgram->link())
    {
        BR_EXIT_FAILURE("Shader::Initialize: Could not link shader program.");
    }

    if (!mProgram->bind())
    {
        BR_EXIT_FAILURE("Shader::Initialize: Could not bind shader program.");
    }

    LOG_INFO("Shader::Initialize: '{}' has been initialized.", mName.toStdString());
}

bool BSplineRenderer::Shader::Bind()
{
    return mProgram->bind();
}

void BSplineRenderer::Shader::Release()
{
    mProgram->release();
}

void BSplineRenderer::Shader::AddPath(QOpenGLShader::ShaderTypeBit type, const QString& path)
{
    mPaths.emplace(type, path);
}

QString BSplineRenderer::Shader::GetName() const
{
    return mName;
}

QString BSplineRenderer::Shader::GetShaderTypeString(QOpenGLShader::ShaderTypeBit type)
{
    switch (type)
    {
        case QOpenGLShader::Vertex:
            return "Vertex Shader";
        case QOpenGLShader::Fragment:
            return "Fragment Shader";
        case QOpenGLShader::Geometry:
            return "Geometry Shader";
        case QOpenGLShader::TessellationControl:
            return "Tessellation Control Shader";
        case QOpenGLShader::TessellationEvaluation:
            return "Tesselation Evaluation Shader";
        case QOpenGLShader::Compute:
            return "Compute Shader";
        default:
            return "Unknown Shader";
    }
}

void BSplineRenderer::Shader::SetSampler(const QString& name, GLuint unit, GLuint textureId, GLuint target)
{
    SetUniformValue(name, unit);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, textureId);
}
