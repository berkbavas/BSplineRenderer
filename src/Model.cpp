#include "Model.h"

BSplineCurves3D::Model::Model(QObject* parent)
    : Node(parent)
    , mVisible(true)
{}

BSplineCurves3D::Model::~Model() {}

const QVector<BSplineCurves3D::Model::Type> BSplineCurves3D::Model::ALL_MODEL_TYPES = {
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