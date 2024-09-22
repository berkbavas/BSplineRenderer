#include "Spline.h"

#include <QDebug>

BSplineCurves3D::Spline::Spline(QObject* parent)
    : Curve(parent)
    , mPointRemovedOrAdded(true)
{}

BSplineCurves3D::Spline::~Spline() {}

void BSplineCurves3D::Spline::AddKnotPoint(KnotPoint* knotPoint)
{
    mKnotPoints << knotPoint;
    knotPoint->setParent(this);

    mDirty = true;
    mPointRemovedOrAdded = true;
}

void BSplineCurves3D::Spline::RemoveKnotPoint(KnotPoint* knotPoint)
{
    mKnotPoints.removeAll(knotPoint);

    if (knotPoint)
    {
        knotPoint->setParent(nullptr);
        knotPoint->deleteLater();
    }

    mDirty = true;
    mPointRemovedOrAdded = true;
}

void BSplineCurves3D::Spline::RecreateBezierPatches()
{
    for (auto& patch : mBezierPatches)
    {
        patch->deleteLater();
    }

    mBezierPatches.clear();

    for (int i = 0; i < mKnotPoints.size() - 1; ++i)
    {
        Bezier* patch = new Bezier;
        patch->setParent(this);
        mBezierPatches << patch;
    }
}

BSplineCurves3D::KnotPoint* BSplineCurves3D::Spline::GetClosestKnotPointToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance)
{
    float minDistance = std::numeric_limits<float>::infinity();
    KnotPoint* closestPoint = nullptr;

    for (auto& point : mKnotPoints)
    {
        QVector3D difference = point->GetPosition() - rayOrigin;

        float dot = QVector3D::dotProduct(difference, rayDirection);

        if (dot >= 0.0f)
        {
            float distance = (difference - rayDirection * dot).length();
            if (distance < minDistance)
            {
                minDistance = distance;
                closestPoint = point;
            }
        }
    }

    if (minDistance >= maxDistance)
        closestPoint = nullptr;

    return closestPoint;
}

void BSplineCurves3D::Spline::Update()
{
    if (mPointRemovedOrAdded)
        RecreateBezierPatches();

    for (auto& patch : mBezierPatches)
        patch->RemoveAllControlPoints();

    if (mKnotPoints.size() == 2)
    {
        for (int i = 0; i < mKnotPoints.size(); ++i)
            mBezierPatches[0]->AddControlPoint(new ControlPoint(mKnotPoints.at(i)->GetPosition()));
    }
    else if (mKnotPoints.size() == 3)
    {
        for (int i = 0; i < 2; i++)
        {
            ControlPoint* cp0 = new ControlPoint(mKnotPoints.at(i)->GetPosition());
            ControlPoint* cp1 = new ControlPoint((2.0f / 3.0f) * mKnotPoints.at(i)->GetPosition() + (1.0f / 3.0f) * mKnotPoints.at(i + 1)->GetPosition());
            ControlPoint* cp2 = new ControlPoint((1.0f / 3.0f) * mKnotPoints.at(i)->GetPosition() + (2.0f / 3.0f) * mKnotPoints.at(i + 1)->GetPosition());
            ControlPoint* cp3 = new ControlPoint(mKnotPoints.at(i + 1)->GetPosition());

            mBezierPatches[i]->AddControlPoint(cp0);
            mBezierPatches[i]->AddControlPoint(cp1);
            mBezierPatches[i]->AddControlPoint(cp2);
            mBezierPatches[i]->AddControlPoint(cp3);
        }
    }
    else if (mKnotPoints.size() >= 4)
    {
        QVector<QVector3D> splineControlPoints = GetSplineControlPoints();

        for (int i = 1; i < mKnotPoints.size(); ++i)
        {
            ControlPoint* cp0 = new ControlPoint(mKnotPoints.at(i - 1)->GetPosition());
            ControlPoint* cp1 = new ControlPoint((2.0f / 3.0f) * splineControlPoints[i - 1] + (1.0f / 3.0f) * splineControlPoints[i]);
            ControlPoint* cp2 = new ControlPoint((1.0f / 3.0f) * splineControlPoints[i - 1] + (2.0f / 3.0f) * splineControlPoints[i]);
            ControlPoint* cp3 = new ControlPoint(mKnotPoints.at(i)->GetPosition());

            mBezierPatches[i - 1]->AddControlPoint(cp0);
            mBezierPatches[i - 1]->AddControlPoint(cp1);
            mBezierPatches[i - 1]->AddControlPoint(cp2);
            mBezierPatches[i - 1]->AddControlPoint(cp3);
        }
    }

    mPointRemovedOrAdded = false;
    mDirty = false;
}

QVector3D BSplineCurves3D::Spline::ValueAt(float t) const
{
    int n = qMin((int)t, mBezierPatches.size() - 1);

    return mBezierPatches[n]->ValueAt(t - n);
}

QVector3D BSplineCurves3D::Spline::TangentAt(float t) const
{
    int n = qMin((int)t, mBezierPatches.size() - 1);

    return mBezierPatches[n]->TangentAt(t - n);
}

void BSplineCurves3D::Spline::Translate(const QVector3D& translation)
{
    for (auto& point : mKnotPoints)
    {
        point->SetPosition(point->GetPosition() + translation);
    }

    mDirty = true;
}

float BSplineCurves3D::Spline::ClosestDistanceToRay(const QVector3D& cameraPosition, const QVector3D& rayDirection, float epsilon)
{
    if (mDirty)
        Update();

    float minDistance = std::numeric_limits<float>::infinity();

    for (auto& patch : mBezierPatches)
    {
        float distance = patch->ClosestDistanceToRay(cameraPosition, rayDirection, epsilon);
        if (distance < minDistance)
        {
            minDistance = distance;
        }
    }

    return minDistance;
}

float BSplineCurves3D::Spline::Length()
{
    if (mDirty)
        Update();

    float length = 0.0f;

    for (auto& patch : mBezierPatches)
    {
        length += patch->Length();
    }

    return length;
}

Eigen::MatrixXf BSplineCurves3D::Spline::CreateCoefficientMatrix()
{
    int n = mKnotPoints.size() - 2;
    Eigen::MatrixXf coef(n, n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            coef(i, j) = 0;
        }
    }

    // First row
    coef(0, 0) = 4;
    coef(0, 1) = 1;

    for (int i = 1; i < n - 1; ++i)
    {
        coef(i, i - 1) = 1;
        coef(i, i) = 4;
        coef(i, i + 1) = 1;
    }

    // Last row
    coef(n - 1, n - 2) = 1;
    coef(n - 1, n - 1) = 4;

    return coef;
}

QVector<QVector3D> BSplineCurves3D::Spline::GetSplineControlPoints()
{
    int n = mKnotPoints.size();

    Eigen::MatrixXf knotPoints(n, 3);

    for (int i = 0; i < n; ++i)
    {
        knotPoints(i, 0) = mKnotPoints.at(i)->GetPosition().x();
        knotPoints(i, 1) = mKnotPoints.at(i)->GetPosition().y();
        knotPoints(i, 2) = mKnotPoints.at(i)->GetPosition().z();
    }

    // Constants on the right side
    Eigen::MatrixXf constants(n - 2, 3);

    for (int j = 0; j < 3; ++j)
    {
        constants(0, j) = 6 * knotPoints(1, j) - knotPoints(0, j);
        constants(n - 3, j) = 6 * knotPoints(n - 2, j) - knotPoints(n - 1, j);
    }

    for (int i = 1; i < n - 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            constants(i, j) = 6 * knotPoints(i + 1, j);
        }
    }

    // Compute BSpline control points
    Eigen::MatrixXf coef = CreateCoefficientMatrix();
    Eigen::MatrixXf controlPoints = coef.inverse() * constants;

    // Result
    QVector<QVector3D> result(n, QVector3D());
    result[0] = QVector3D(knotPoints(0, 0), knotPoints(0, 1), knotPoints(0, 2));
    result[n - 1] = QVector3D(knotPoints(n - 1, 0), knotPoints(n - 1, 1), knotPoints(n - 1, 2));
    for (int i = 0; i < n - 2; ++i)
        result[i + 1] = QVector3D(controlPoints(i, 0), controlPoints(i, 1), controlPoints(i, 2));

    return result;
}

float BSplineCurves3D::Spline::GetRadius() const
{
    return mRadius;
}

void BSplineCurves3D::Spline::SetRadius(float newRadius)
{
    mRadius = newRadius;

    for (auto& patch : mBezierPatches)
        patch->SetRadius(mRadius);
}

int BSplineCurves3D::Spline::GetSectorCount() const
{
    return mSectorCount;
}

void BSplineCurves3D::Spline::SetSectorCount(int newSectorCount)
{
    mSectorCount = newSectorCount;

    for (auto& patch : mBezierPatches)
        patch->SetSectorCount(mSectorCount);
}

const QList<BSplineCurves3D::KnotPoint*>& BSplineCurves3D::Spline::GetKnotPoints()
{
    if (mDirty)
        Update();

    return mKnotPoints;
}

BSplineCurves3D::Spline* BSplineCurves3D::Spline::DeepCopy()
{
    Spline* copy = new Spline;

    for (auto& point : mKnotPoints)
        copy->AddKnotPoint(new KnotPoint(point->GetPosition()));

    return copy;
}

const QList<BSplineCurves3D::Bezier*>& BSplineCurves3D::Spline::GetBezierPatches()
{
    if (mDirty)
        Update();

    return mBezierPatches;
}