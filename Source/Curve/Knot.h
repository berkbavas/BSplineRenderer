#pragma once

#include <QVector3D>
#include <memory>

namespace BSplineRenderer
{
    class Knot
    {
      public:
        Knot(float x, float y, float z);
        Knot(const QVector3D& position);

        void SetPosition(const QVector3D& position);
        void SetPosition(float x, float y, float z);

        const QVector3D& GetPosition() const { return mPosition; }

      private:
        QVector3D mPosition;
    };

    using KnotPtr = std::shared_ptr<Knot>;
}
