#pragma once

#include "Curve/Spline.h"

#include <QVector>

namespace BSplineRenderer
{
    class CurveContainer
    {
      public:
        CurveContainer() = default;

        void AddCurve(SplinePtr spline);
        void RemoveCurve(SplinePtr spline);
        SplinePtr GetCurve(int index);

        const QVector<SplinePtr>& GetCurves() const { return mCurves; }

      private:
        QVector<SplinePtr> mCurves;
    };
}