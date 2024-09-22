#include "CurveManager.h"
#include <QDebug>

BSplineCurves3D::CurveManager::CurveManager(QObject* parent)
    : QObject(parent)
    , mSelectedCurve(nullptr)
    , mSelectedPoint(nullptr)
    , mGlobalPipeRadius(0.125f)
    , mGlobalPipeSectorCount(128)
{}

BSplineCurves3D::CurveManager* BSplineCurves3D::CurveManager::Instance()
{
    static CurveManager instance;

    return &instance;
}

const QList<BSplineCurves3D::Spline*>& BSplineCurves3D::CurveManager::GetCurves() const
{
    return mCurves;
}

QList<BSplineCurves3D::Spline*>& BSplineCurves3D::CurveManager::GetCurvesNonConst()
{
    return mCurves;
}

void BSplineCurves3D::CurveManager::AddCurve(Spline* curve)
{
    mCurves << curve;
}

void BSplineCurves3D::CurveManager::RemoveCurve(Spline* curve)
{
    if (curve == mSelectedCurve)
        SetSelectedCurve(nullptr);

    mCurves.removeAll(curve);
    curve->deleteLater();
}

void BSplineCurves3D::CurveManager::RemoveAllCurves()
{
    for (auto& curve : mCurves)
    {
        curve->deleteLater();
    }

    mCurves.clear();
    SetSelectedCurve(nullptr);
    SetSelectedKnotPoint(nullptr);
}

void BSplineCurves3D::CurveManager::AddCurves(QList<Spline*> curves)
{
    for (auto& curve : curves)
    {
        mCurves << curve;
    }
}

BSplineCurves3D::Spline* BSplineCurves3D::CurveManager::SelectCurve(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance)
{
    float minDistance = std::numeric_limits<float>::infinity();
    Spline* selectedCurve = nullptr;

    for (auto& curve : mCurves)
    {
        float distance = curve->ClosestDistanceToRay(rayOrigin, rayDirection);

        if (distance < minDistance)
        {
            minDistance = distance;
            selectedCurve = curve;
        }
    }

    if (minDistance >= maxDistance)
    {
        selectedCurve = nullptr;
    }

    SetSelectedCurve(selectedCurve);

    return selectedCurve;
}

BSplineCurves3D::KnotPoint* BSplineCurves3D::CurveManager::SelectKnotPoint(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance)
{
    if (mSelectedCurve)
    {
        KnotPoint* point = mSelectedCurve->GetClosestKnotPointToRay(rayOrigin, rayDirection, maxDistance);
        SetSelectedKnotPoint(point);
        return point;
    }

    return nullptr;
}

BSplineCurves3D::Spline* BSplineCurves3D::CurveManager::GetSelectedCurve() const
{
    return mSelectedCurve;
}

void BSplineCurves3D::CurveManager::SetSelectedCurve(Spline* newSelectedCurve)
{
    if (mSelectedCurve == newSelectedCurve)
        return;

    if (mSelectedCurve)
        mSelectedCurve->SetSelected(false);

    if (newSelectedCurve)
        newSelectedCurve->SetSelected(true);

    mSelectedCurve = newSelectedCurve;

    emit SelectedCurveChanged(mSelectedCurve);

    SetSelectedKnotPoint(nullptr);
}

BSplineCurves3D::KnotPoint* BSplineCurves3D::CurveManager::GetSelectedKnotPoint() const
{
    return mSelectedPoint;
}

void BSplineCurves3D::CurveManager::SetSelectedKnotPoint(KnotPoint* newSelectedPoint)
{
    if (mSelectedPoint == newSelectedPoint)
        return;

    if (mSelectedPoint)
        mSelectedPoint->SetSelected(false);

    if (newSelectedPoint)
        newSelectedPoint->SetSelected(true);

    mSelectedPoint = newSelectedPoint;
    emit SelectedKnotPointChanged(mSelectedPoint);
}

float BSplineCurves3D::CurveManager::GetGlobalPipeRadius() const
{
    return mGlobalPipeRadius;
}

void BSplineCurves3D::CurveManager::SetGlobalPipeRadius(float newGlobalPipeRadius)
{
    mGlobalPipeRadius = newGlobalPipeRadius;

    for (auto& curve : mCurves)
        curve->SetRadius(mGlobalPipeRadius);
}

int BSplineCurves3D::CurveManager::GetGlobalPipeSectorCount() const
{
    return mGlobalPipeSectorCount;
}

void BSplineCurves3D::CurveManager::SetGlobalPipeSectorCount(int newGlobalPipeSectorCount)
{
    mGlobalPipeSectorCount = newGlobalPipeSectorCount;

    for (auto& curve : mCurves)
        curve->SetSectorCount(mGlobalPipeSectorCount);
}