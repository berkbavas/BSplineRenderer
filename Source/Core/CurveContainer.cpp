#include "CurveContainer.h"

void BSplineRenderer::CurveContainer::AddCurve(SplinePtr spline)
{
    mCurves << spline;
}

void BSplineRenderer::CurveContainer::RemoveCurve(SplinePtr spline)
{
    mCurves.removeAll(spline);
}

BSplineRenderer::SplinePtr BSplineRenderer::CurveContainer::GetCurve(int index)
{
    return mCurves[index];
}
