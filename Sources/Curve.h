#pragma once

#include "Material.h"
#include "Macros.h"

#include <QObject>

namespace BSplineCurves3D
{
    class Curve : public QObject
    {
        Q_OBJECT
    public:
        explicit Curve(QObject* parent = nullptr);
        virtual ~Curve();

        virtual void Update() = 0;
        virtual float ClosestDistanceToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float epsilon = 0.01) = 0;
        virtual QVector3D ValueAt(float t) const = 0;
        virtual QVector3D TangentAt(float t) const = 0;
        virtual void Translate(const QVector3D& translation) = 0;
        virtual float Length() = 0;

        DEFINE_MEMBER(bool, Selected);
        DEFINE_MEMBER(bool, Dirty);
        DEFINE_MEMBER(Material, Material);
    };
}
