#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

namespace BSplineCurves3D
{
    class Ticks : protected QOpenGLFunctions
    {
    public:
        Ticks(float startingPoint, float endPoint, int size);
        ~Ticks();

        void Create();
        void Render();

        float GetStartingPoint() const;
        float GetEndPoint() const;
        int GetSize() const;
        float GetTicksDelta() const;

    private:
        QOpenGLVertexArrayObject mTicksVertexArray;
        QOpenGLBuffer mTicksBuffer;

        QVector<float> mTicks;

        const float mStartingPoint;
        const float mEndPoint;
        const int mSize;
        const float mTicksDelta;
    };
}
