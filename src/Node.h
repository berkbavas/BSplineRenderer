#pragma once

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>

namespace BSplineCurves3D
{
    class Node : public QObject
    {
        Q_OBJECT

    public:
        explicit Node(QObject* parent = nullptr);
        virtual ~Node();

        virtual const QMatrix4x4& Transformation() const;
        virtual void SetTransformation(const QMatrix4x4& newTransformation);

        virtual const QQuaternion& Rotation() const;
        virtual void SetRotation(const QQuaternion& newRotation);

        virtual const QVector3D& Position() const;
        virtual void SetPosition(const QVector3D& newPosition);

        virtual const QVector3D& Scale() const;
        virtual void SetScale(const QVector3D& newScale);

    private:
        virtual void UpdateTransformation();

    protected:
        QMatrix4x4 mTransformation;
        QQuaternion mRotation;
        QVector3D mPosition;
        QVector3D mScale;
    };
}
