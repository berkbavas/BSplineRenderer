#pragma once

#include "Material.h"
#include "Node.h"

#include <QOpenGLTexture>

namespace BSplineCurves3D
{
    class Model : public Node
    {
    public:
        explicit Model(QObject* parent = nullptr);
        virtual ~Model();

        enum Type { //
            Capsule,
            Cone,
            Cube,
            Cylinder,
            Dome,
            Plane,
            Pyramid,
            Sphere,
            Tetrahedron,
            Torus,
            TorusKnot,
        };

        static const QVector<Model::Type> ALL_MODEL_TYPES;

        DEFINE_MEMBER(Type, Type);
        DEFINE_MEMBER(Material, Material);
        DEFINE_MEMBER(bool, Visible);
    };
}
