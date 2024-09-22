#pragma once

#include "Point.h"
#include "Spline.h"

#include <QObject>

namespace BSplineCurves3D
{
    class CurveManager : public QObject
    {
        Q_OBJECT
    private:
        explicit CurveManager(QObject* parent = nullptr);

    public:
        static CurveManager* Instance();

        const QList<Spline*>& GetCurves() const;
        QList<Spline*>& GetCurvesNonConst();

        void AddCurve(Spline* curve);
        void RemoveCurve(Spline* curve);
        void RemoveAllCurves();
        void AddCurves(QList<Spline*> curves);

        Spline* SelectCurve(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance = 0.5f);
        ControlPoint* SelectKnotPoint(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance = 0.5f);

        Spline* GetSelectedCurve() const;
        void SetSelectedCurve(Spline* newSelectedCurve);

        Point* GetSelectedKnotPoint() const;
        void SetSelectedKnotPoint(Point* newSelectedPoint);

        float GetGlobalPipeRadius() const;
        void SetGlobalPipeRadius(float newGlobalPipeRadius);

        int GetGlobalPipeSectorCount() const;
        void SetGlobalPipeSectorCount(int newGlobalPipeSectorCount);

    signals:
        void SelectedCurveChanged(Spline* curve);
        void SelectedKnotPointChanged(KnotPoint* point);

    private:
        QList<Spline*> mCurves;
        Spline* mSelectedCurve;
        KnotPoint* mSelectedPoint;
        float mGlobalPipeRadius;
        int mGlobalPipeSectorCount;
    };
}
