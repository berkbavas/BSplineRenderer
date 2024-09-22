#pragma once

#include <QObject>
#include <QVector3D>

namespace BSplineCurves3D
{
    class Point : public QObject
    {
        Q_OBJECT
    public:
        explicit Point(QObject* parent = nullptr);
        explicit Point(float x, float y, float z = 0.0f, QObject* parent = nullptr);
        explicit Point(const QVector3D& position, QObject* parent = nullptr);

        bool GetSelected() const;
        void SetSelected(bool newSelected);

        const QVector3D& GetPosition() const;
        void SetPosition(const QVector3D& newPosition);

    private:
        bool mSelected;
        QVector3D mPosition;
    };

    typedef Point ControlPoint;
    typedef Point KnotPoint;
}
