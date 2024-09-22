#include "ShaderManager.h"
#include "Helper.h"

BSplineCurves3D::ShaderManager::ShaderManager(QObject* parent)
    : QObject(parent)
    , mActiveShader(Shader::None)
{}

bool BSplineCurves3D::ShaderManager::Init()
{
    initializeOpenGLFunctions();

    // Basic
    {
        QOpenGLShaderProgram* shader = new QOpenGLShaderProgram;
        mPrograms.insert(Shader::Basic, shader);

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::GetBytes(":/Resources/Shaders/Basic.vert")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load vertex shader.";
            return false;
        }

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::GetBytes(":/Resources/Shaders/Basic.frag")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load fragment shader.";
            return false;
        }

        if (!shader->link())
        {
            qWarning() << Q_FUNC_INFO << "Could not link shader program.";
            return false;
        }

        if (!shader->bind())
        {
            qWarning() << Q_FUNC_INFO << "Could not bind shader program.";
            return false;
        }

        QMap<QString, GLuint> locations;

        locations.insert("light.color", shader->uniformLocation("light.color"));
        locations.insert("light.position", shader->uniformLocation("light.position"));
        locations.insert("light.ambient", shader->uniformLocation("light.ambient"));
        locations.insert("light.diffuse", shader->uniformLocation("light.diffuse"));
        locations.insert("light.specular", shader->uniformLocation("light.specular"));

        locations.insert("node.transformation", shader->uniformLocation("node.transformation"));
        locations.insert("node.color", shader->uniformLocation("node.color"));
        locations.insert("node.ambient", shader->uniformLocation("node.ambient"));
        locations.insert("node.diffuse", shader->uniformLocation("node.diffuse"));
        locations.insert("node.specular", shader->uniformLocation("node.specular"));
        locations.insert("node.shininess", shader->uniformLocation("node.shininess"));

        locations.insert("camera_position", shader->uniformLocation("camera_position"));
        locations.insert("view_matrix", shader->uniformLocation("view_matrix"));
        locations.insert("projection_matrix", shader->uniformLocation("projection_matrix"));

        shader->bindAttributeLocation("position", 0);
        shader->bindAttributeLocation("normal", 1);

        shader->release();

        mLocations.insert(Shader::Basic, locations);

        qInfo() << Q_FUNC_INFO << "BasicShader is initialized.";
        qInfo() << Q_FUNC_INFO << "Uniform locations are:" << locations;
    }

    // Path
    {
        QOpenGLShaderProgram* shader = new QOpenGLShaderProgram;
        mPrograms.insert(Shader::Path, shader);

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::GetBytes(":/Resources/Shaders/Path.vert")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load vertex shader.";
            return false;
        }

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::GetBytes(":/Resources/Shaders/Path.frag")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load fragment shader.";
            return false;
        }

        if (!shader->link())
        {
            qWarning() << Q_FUNC_INFO << "Could not link shader program.";
            return false;
        }

        if (!shader->bind())
        {
            qWarning() << Q_FUNC_INFO << "Could not bind shader program.";
            return false;
        }

        QMap<QString, GLuint> locations;

        locations.insert("control_points", shader->uniformLocation("control_points"));
        locations.insert("control_points_count", shader->uniformLocation("control_points_count"));
        locations.insert("color", shader->uniformLocation("color"));

        locations.insert("view_matrix", shader->uniformLocation("view_matrix"));
        locations.insert("projection_matrix", shader->uniformLocation("projection_matrix"));

        shader->bindAttributeLocation("t", 0);
        shader->release();

        mLocations.insert(Shader::Path, locations);

        qInfo() << Q_FUNC_INFO << "PathShader is initialized.";
        qInfo() << Q_FUNC_INFO << "Uniform locations are:" << locations;
    }

    // PipeDumb
    {
        QOpenGLShaderProgram* shader = new QOpenGLShaderProgram;
        mPrograms.insert(Shader::PipeDumb, shader);

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::GetBytes(":/Resources/Shaders/PipeDumb.vert")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load vertex shader.";
            return false;
        }

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Geometry, Helper::GetBytes(":/Resources/Shaders/PipeDumb.geom")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load geometry shader.";
            return false;
        }

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::GetBytes(":/Resources/Shaders/PipeDumb.frag")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load fragment shader.";
            return false;
        }

        if (!shader->link())
        {
            qWarning() << Q_FUNC_INFO << "Could not link shader program.";
            return false;
        }

        if (!shader->bind())
        {
            qWarning() << Q_FUNC_INFO << "Could not bind shader program.";
            return false;
        }

        QMap<QString, GLuint> locations;

        locations.insert("view_matrix", shader->uniformLocation("view_matrix"));
        locations.insert("projection_matrix", shader->uniformLocation("projection_matrix"));
        locations.insert("camera_position", shader->uniformLocation("camera_position"));

        locations.insert("control_points", shader->uniformLocation("control_points"));
        locations.insert("control_points_count", shader->uniformLocation("control_points_count"));

        locations.insert("dt", shader->uniformLocation("dt"));
        locations.insert("r", shader->uniformLocation("r"));
        locations.insert("sector_angle_0", shader->uniformLocation("sector_angle_0"));
        locations.insert("sector_angle_1", shader->uniformLocation("sector_angle_1"));

        locations.insert("light.color", shader->uniformLocation("light.color"));
        locations.insert("light.position", shader->uniformLocation("light.position"));
        locations.insert("light.ambient", shader->uniformLocation("light.ambient"));
        locations.insert("light.diffuse", shader->uniformLocation("light.diffuse"));
        locations.insert("light.specular", shader->uniformLocation("light.specular"));

        locations.insert("node.color", shader->uniformLocation("node.color"));
        locations.insert("node.ambient", shader->uniformLocation("node.ambient"));
        locations.insert("node.diffuse", shader->uniformLocation("node.diffuse"));
        locations.insert("node.specular", shader->uniformLocation("node.specular"));
        locations.insert("node.shininess", shader->uniformLocation("node.shininess"));

        shader->bindAttributeLocation("t", 0);
        shader->release();

        mLocations.insert(Shader::PipeDumb, locations);

        qInfo() << Q_FUNC_INFO << "PipeDumb is initialized.";
        qInfo() << Q_FUNC_INFO << "Uniform locations are:" << locations;
    }

    // PipeSmart
    {
        QOpenGLShaderProgram* shader = new QOpenGLShaderProgram;
        mPrograms.insert(Shader::PipeSmart, shader);

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::GetBytes(":/Resources/Shaders/PipeSmart.vert")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load vertex shader.";
            return false;
        }

        if (!shader->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::GetBytes(":/Resources/Shaders/PipeSmart.frag")))
        {
            qWarning() << Q_FUNC_INFO << "Could not load fragment shader.";
            return false;
        }

        if (!shader->link())
        {
            qWarning() << Q_FUNC_INFO << "Could not link shader program.";
            return false;
        }

        if (!shader->bind())
        {
            qWarning() << Q_FUNC_INFO << "Could not bind shader program.";
            return false;
        }

        QMap<QString, GLuint> locations;

        locations.insert("light.color", shader->uniformLocation("light.color"));
        locations.insert("light.position", shader->uniformLocation("light.position"));
        locations.insert("light.ambient", shader->uniformLocation("light.ambient"));
        locations.insert("light.diffuse", shader->uniformLocation("light.diffuse"));
        locations.insert("light.specular", shader->uniformLocation("light.specular"));

        locations.insert("node.color", shader->uniformLocation("node.color"));
        locations.insert("node.ambient", shader->uniformLocation("node.ambient"));
        locations.insert("node.diffuse", shader->uniformLocation("node.diffuse"));
        locations.insert("node.specular", shader->uniformLocation("node.specular"));
        locations.insert("node.shininess", shader->uniformLocation("node.shininess"));

        locations.insert("camera_position", shader->uniformLocation("camera_position"));
        locations.insert("view_matrix", shader->uniformLocation("view_matrix"));
        locations.insert("projection_matrix", shader->uniformLocation("projection_matrix"));

        shader->bindAttributeLocation("position", 0);
        shader->bindAttributeLocation("normal", 1);

        shader->release();

        mLocations.insert(Shader::PipeSmart, locations);

        qInfo() << Q_FUNC_INFO << "PipeSmart is initialized.";
        qInfo() << Q_FUNC_INFO << "Uniform locations are:" << locations;
    }

    return true;
}

bool BSplineCurves3D::ShaderManager::Bind(Shader shader)
{
    mActiveShader = shader;
    return mPrograms.value(mActiveShader)->bind();
}

void BSplineCurves3D::ShaderManager::Release()
{
    mPrograms.value(mActiveShader)->release();
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, int value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, float value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, const QVector3D& value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, const QVector4D& value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, const QMatrix4x4& value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValue(const QString& name, const QMatrix3x3& value)
{
    mPrograms.value(mActiveShader)->setUniformValue(mLocations.value(mActiveShader).value(name), value);
}

void BSplineCurves3D::ShaderManager::SetUniformValueArray(const QString& name, const QVector<QVector3D>& values)
{
    mPrograms.value(mActiveShader)->setUniformValueArray(mLocations.value(mActiveShader).value(name), values.constData(), values.size());
}

BSplineCurves3D::ShaderManager* BSplineCurves3D::ShaderManager::Instance()
{
    static ShaderManager instance;

    return &instance;
}