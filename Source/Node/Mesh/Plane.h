#pragma once

#include "Node/Mesh/Mesh.h"

namespace BSplineRenderer
{
    class Plane : public Mesh
    {
      public:
        Plane(float size);
        ~Plane();

        void Render() override;

      private:
        GLuint mVertexArray{ 0 };
        GLuint mVertexBuffer{ 0 };

        QVector<Vertex> mVertices;
    };

    using PlanePtr = std::shared_ptr<Plane>;
}