#include "Spline.h"

#include "Util/Logger.h"

void BSplineRenderer::Spline::AddKnot(KnotPtr knot)
{
    mKnots << knot;
    MakeDirty();
}

BSplineRenderer::KnotPtr BSplineRenderer::Spline::AddKnot(float x, float y, float z)
{
    return AddKnot(QVector3D(x, y, z));
}

BSplineRenderer::KnotPtr BSplineRenderer::Spline::AddKnot(const QVector3D& position)
{
    KnotPtr knot = std::make_shared<Knot>(position);
    mKnots << knot;
    MakeDirty();
    return knot;
}

void BSplineRenderer::Spline::MakeDirty()
{
    mDirty = true;
}

void BSplineRenderer::Spline::UpdateIfDirty()
{
    if (IsDirty())
    {
        Update();
    }
}

void BSplineRenderer::Spline::Render()
{
    UpdateIfDirty();
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_PATCHES, 0, mBezierControlPoints.size());
}

void BSplineRenderer::Spline::Update()
{
    mBezierControlPoints.clear();

    if (mKnots.size() == 1)
    {
        mBezierControlPoints << mKnots[0]->GetPosition();
        mBezierControlPoints << mKnots[0]->GetPosition();
        mBezierControlPoints << mKnots[0]->GetPosition();
        mBezierControlPoints << mKnots[0]->GetPosition();
    }
    else if (mKnots.size() == 2)
    {
        mBezierControlPoints << mKnots[0]->GetPosition();
        mBezierControlPoints << mKnots[0]->GetPosition();
        mBezierControlPoints << mKnots[1]->GetPosition();
        mBezierControlPoints << mKnots[1]->GetPosition();
    }
    else if (mKnots.size() == 3)
    {
        for (int i = 0; i < 2; i++)
        {
            mBezierControlPoints << mKnots.at(i)->GetPosition();
            mBezierControlPoints << (2.0f / 3.0f) * mKnots.at(i)->GetPosition() + (1.0f / 3.0f) * mKnots.at(i + 1)->GetPosition();
            mBezierControlPoints << (1.0f / 3.0f) * mKnots.at(i)->GetPosition() + (2.0f / 3.0f) * mKnots.at(i + 1)->GetPosition();
            mBezierControlPoints << mKnots.at(i + 1)->GetPosition();
        }
    }
    else if (mKnots.size() >= 4)
    {
        UpdateSplineControlPoints();

        for (int i = 1; i < mKnots.size(); ++i)
        {
            mBezierControlPoints << mKnots.at(i - 1)->GetPosition();
            mBezierControlPoints << (2.0f / 3.0f) * mSplineControlPoints[i - 1] + (1.0f / 3.0f) * mSplineControlPoints[i];
            mBezierControlPoints << (1.0f / 3.0f) * mSplineControlPoints[i - 1] + (2.0f / 3.0f) * mSplineControlPoints[i];
            mBezierControlPoints << mKnots.at(i)->GetPosition();
        }
    }

    InitializeOpenGLStuffIfNot();
    DestroyOpenGLStuff();
    ContructOpenGLStuff();
    mDirty = false;
}

void BSplineRenderer::Spline::DestroyOpenGLStuff()
{
    if (mVertexArray != 0)
    {
        glDeleteVertexArrays(1, &mVertexArray);
        mVertexArray = 0;
    }

    if (mVertexBuffer != 0)
    {
        glDeleteBuffers(1, &mVertexBuffer);
        mVertexBuffer = 0;
    }
}

void BSplineRenderer::Spline::ContructOpenGLStuff()
{
    if (mVertexArray != 0 || mVertexBuffer != 0)
    {
        DestroyOpenGLStuff();
    }

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mBezierControlPoints.size() * sizeof(QVector3D), mBezierControlPoints.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*) 0);
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, NUM_OF_PATCH_POINTS);

    if (mVertexArray == 0 || mVertexBuffer == 0)
    {
        BR_EXIT_FAILURE("Spline::ContructOpenGLStuff: OpenGL handle(s) could not be created! this = {:#010x}", reinterpret_cast<intptr_t>(this));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_DEBUG("Spline::ContructOpenGLStuff: OpenGL stuff for Spline has been constructed. "
              "this = {:#010x}, mVertexArray = {}, mVertexBuffer = {}, # of Knots: {}",
              reinterpret_cast<intptr_t>(this), mVertexArray, mVertexBuffer, mKnots.size());
}

void BSplineRenderer::Spline::InitializeOpenGLStuffIfNot()
{
    if (mInitialized == false)
    {
        initializeOpenGLFunctions();
        mInitialized = true;
    }
}

Eigen::MatrixXf BSplineRenderer::Spline::CreateCoefficientMatrix()
{
    int n = mKnots.size() - 2;
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

void BSplineRenderer::Spline::UpdateSplineControlPoints()
{
    int n = mKnots.size();

    Eigen::MatrixXf knotPoints(n, 3);

    for (int i = 0; i < n; ++i)
    {
        knotPoints(i, 0) = mKnots.at(i)->GetPosition().x();
        knotPoints(i, 1) = mKnots.at(i)->GetPosition().y();
        knotPoints(i, 2) = mKnots.at(i)->GetPosition().z();
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
    mSplineControlPoints.clear();
    mSplineControlPoints.resize(n);

    mSplineControlPoints[0] = QVector3D(knotPoints(0, 0), knotPoints(0, 1), knotPoints(0, 2));
    mSplineControlPoints[n - 1] = QVector3D(knotPoints(n - 1, 0), knotPoints(n - 1, 1), knotPoints(n - 1, 2));

    for (int i = 0; i < n - 2; ++i)
    {
        mSplineControlPoints[i + 1] = QVector3D(controlPoints(i, 0), controlPoints(i, 1), controlPoints(i, 2));
    }
}

BSplineRenderer::KnotPtr BSplineRenderer::Spline::GetClosestKnotToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance) const
{
    float minDistance = std::numeric_limits<float>::infinity();
    KnotPtr closestKnot = nullptr;

    for (auto& knot : mKnots)
    {
        QVector3D difference = knot->GetPosition() - rayOrigin;

        float dot = QVector3D::dotProduct(difference, rayDirection);

        if (dot >= 0.0f)
        {
            float distance = (difference - rayDirection * dot).length();
            if (distance < minDistance)
            {
                minDistance = distance;
                closestKnot = knot;
            }
        }
    }

    if (minDistance >= maxDistance)
    {
        closestKnot = nullptr;
    }

    return closestKnot;
}