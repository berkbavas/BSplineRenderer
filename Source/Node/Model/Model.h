#pragma once

#include "Node/Mesh/Mesh.h"
#include "Node/Node.h"

namespace BSplineRenderer
{
    class Model : public Node
    {
      public:
        Model(MeshPtr mesh)
            : mMesh(mesh) {}

        MeshPtr GetMesh() const { return mMesh; }

      private:
        DEFINE_MEMBER(QVector4D, Color, QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
        DEFINE_MEMBER(float, Ambient, 0.25f);
        DEFINE_MEMBER(float, Diffuse, 0.5f);
        DEFINE_MEMBER(float, Specular, 0.25f);
        DEFINE_MEMBER(float, Shininess, 4.0f);

        MeshPtr mMesh;
    };

    using ModelPtr = std::shared_ptr<Model>;
}