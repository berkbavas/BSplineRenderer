#pragma once

#include <QMatrix4x4>
#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

namespace BSplineCurves3D
{
    class ShaderManager : public QObject, protected QOpenGLFunctions
    {
    public:
        explicit ShaderManager(QObject* parent = nullptr);

        enum class Shader { //
            None,
            Basic,
            Path,
            PipeDumb,
            PipeSmart
        };

        bool Init();
        bool Bind(Shader shader);
        void Release();

        void SetUniformValue(const QString& name, int value);
        void SetUniformValue(const QString& name, float value);
        void SetUniformValue(const QString& name, const QVector3D& value);
        void SetUniformValue(const QString& name, const QVector4D& value);
        void SetUniformValue(const QString& name, const QMatrix4x4& value);
        void SetUniformValue(const QString& name, const QMatrix3x3& value);
        void SetUniformValueArray(const QString& name, const QVector<QVector3D>& values);

        static ShaderManager* Instance();

    private:
        Shader mActiveShader;
        QMap<Shader, QOpenGLShaderProgram*> mPrograms;
        QMap<Shader, QMap<QString, GLuint>> mLocations;
    };
}
