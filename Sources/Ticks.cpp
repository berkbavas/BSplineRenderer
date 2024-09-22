#include "Ticks.h"

BSplineCurves3D::Ticks::Ticks(float startingPoint, float endPoint, int size)
    : mStartingPoint(startingPoint)
    , mEndPoint(endPoint)
    , mSize(size)
    , mTicksDelta((mEndPoint - mStartingPoint) / mSize)
{}

BSplineCurves3D::Ticks::~Ticks()
{
    mTicksVertexArray.destroy();
    mTicksBuffer.destroy();
}

void BSplineCurves3D::Ticks::Create()
{
    initializeOpenGLFunctions();

    // Ticks
    mTicks = QVector<float>(mSize, 0.0);
    for (int i = 0; i < mSize; ++i)
        mTicks[i] = mStartingPoint + i * (mEndPoint - mStartingPoint) / mSize;

    // VAO and VBO
    mTicksVertexArray.create();
    mTicksVertexArray.bind();

    mTicksBuffer.create();
    mTicksBuffer.bind();

    mTicksBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mTicksBuffer.allocate(mTicks.constData(), mSize * sizeof(GLfloat));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    mTicksBuffer.release();
    mTicksVertexArray.release();
}

void BSplineCurves3D::Ticks::Render()
{
    mTicksVertexArray.bind();
    glDrawArrays(GL_POINTS, 0, mTicks.size());
    mTicksVertexArray.release();
}

float BSplineCurves3D::Ticks::GetTicksDelta() const
{
    return mTicksDelta;
}

int BSplineCurves3D::Ticks::GetSize() const
{
    return mSize;
}

float BSplineCurves3D::Ticks::GetStartingPoint() const
{
    return mStartingPoint;
}

float BSplineCurves3D::Ticks::GetEndPoint() const
{
    return mEndPoint;
}