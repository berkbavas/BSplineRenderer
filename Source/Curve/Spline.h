#pragma once

#include "Core/Constants.h"
#include "Curve/Knot.h"
#include "Util/Macros.h"

#include <Dense>
#include <QOpenGLExtraFunctions>
#include <QVector>

namespace BSplineRenderer
{
    class Spline : QOpenGLExtraFunctions
    {
      public:
        Spline() = default;

        void AddKnot(KnotPtr knot);
        KnotPtr AddKnot(float x, float y, float z);
        KnotPtr AddKnot(const QVector3D& position);

        const QVector<KnotPtr>& GetKnots() const { return mKnots; }

        void Render();
        void Update();
        void MakeDirty();
        void UpdateIfDirty();
        bool IsDirty() const { return mDirty; }

      private:
        void DestroyOpenGLStuff();
        void ContructOpenGLStuff();
        void InitializeOpenGLStuffIfNot();

        Eigen::MatrixXf CreateCoefficientMatrix();
        void UpdateSplineControlPoints();

        QVector<KnotPtr> mKnots;
        QVector<QVector3D> mSplineControlPoints;
        QVector<QVector3D> mBezierControlPoints;
        GLuint mVertexArray{ 0 };
        GLuint mVertexBuffer{ 0 };

        bool mDirty{ false };

        bool mInitialized{ false };

        DEFINE_MEMBER(QVector4D, Color, QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
        DEFINE_MEMBER(float, Ambient, 0.25f);
        DEFINE_MEMBER(float, Diffuse, 0.75f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);
        DEFINE_MEMBER(float, Radius, DEFAULT_RADIUS);

        static constexpr int NUM_OF_PATCH_POINTS = 4;
    };

    using SplinePtr = std::shared_ptr<Spline>;
}