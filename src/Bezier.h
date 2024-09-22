#pragma once

#include "Curve.h"
#include "Point.h"

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

namespace BSplineCurves3D
{
    class Bezier : public Curve, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit Bezier(QObject* parent = nullptr);
        virtual ~Bezier();

        enum class VertexGenerationStatus {
            GeneratingVertices,
            WaitingForOpenGLUpdate,
            Ready,
            Dirty,
        };

        void AddControlPoint(ControlPoint* controlPoint);
        void RemoveControlPoint(ControlPoint* controlPoint);

        void InsertControlPoint(int index, ControlPoint* controlPoint);
        void RemoveControlPoint(int index);
        void RemoveAllControlPoints();

        const QList<ControlPoint*>& GetControlPoints() const;
        QVector<QVector3D> GetControlPointPositions();
        ControlPoint* GetClosestControlPointToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float maxDistance = 0.5f);

        int GetDegree() const;
        float Factorial(int n) const;
        float Choose(int n, int k) const;

        // Curve interface
        virtual void Update() override;
        virtual QVector3D ValueAt(float t) const override;
        virtual QVector3D TangentAt(float t) const override;
        virtual float ClosestDistanceToRay(const QVector3D& rayOrigin, const QVector3D& rayDirection, float epsilon = 0.01f) override;
        virtual void Translate(const QVector3D& translation) override;
        virtual float Length() override;

        void GenerateVertices();
        void InitializeOpenGLStuff();
        void UpdateOpenGLStuff();
        void Render();

        int GetSectorCount() const;
        void SetSectorCount(int newSectorCount);

        float GetRadius() const;
        void SetRadius(float newRadius);

        VertexGenerationStatus GetVertexGenerationStatus() const;
        void SetVertexGenerationStatus(VertexGenerationStatus newVertexGenerationStatus);

        bool GetInitialized() const;
        void SetInitialized(bool newInitialized);

    private:
        QList<ControlPoint*> mControlPoints;
        float mLength;
        int mTickCount;
        int mSectorCount;
        float mRadius;

        QOpenGLVertexArrayObject mVertexArray;
        QOpenGLBuffer mVertexBuffer;
        QOpenGLBuffer mNormalBuffer;

        QVector<QVector3D> mVertices;
        QVector<QVector3D> mNormals;

        VertexGenerationStatus mVertexGenerationStatus;

        bool mInitialized;
    };
}
