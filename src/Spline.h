#pragma once

#include "Bezier.h"
#include "Curve.h"
#include "Point.h"

#include <QObject>
#include <QVector3D>

#include <Dense>

namespace BSplineCurves3D
{
    class Spline : public Curve
    {
        Q_OBJECT
    public:
        explicit Spline(QObject* parent = nullptr);
        ~Spline();

        void AddKnotPoint(KnotPoint* knotPoint);
        void RemoveKnotPoint(KnotPoint* knotPoint);
        void RecreateBezierPatches();

        KnotPoint* GetClosestKnotPointToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance = 0.5f);

        const QList<Bezier*>& GetBezierPatches();
        const QList<KnotPoint*>& GetKnotPoints();

        Spline* DeepCopy();

        // Curve interface
        void Update();
        QVector3D ValueAt(float t) const;
        QVector3D TangentAt(float t) const;
        void Translate(const QVector3D& translation);
        float ClosestDistanceToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float epsilon = 0.01f);
        float Length();

        int GetSectorCount() const;
        float GetRadius() const;

        void SetSectorCount(int newSectorCount);
        void SetRadius(float newRadius);

    private:
        Eigen::MatrixXf CreateCoefficientMatrix();
        QVector<QVector3D> GetSplineControlPoints();

    private:
        QList<KnotPoint*> mKnotPoints;
        QList<Bezier*> mBezierPatches;

        int mSectorCount;
        float mRadius;

        bool mPointRemovedOrAdded;
    };
}
