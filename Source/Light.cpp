#include "Light.h"

BSplineCurves3D::Light::Light(QObject* parent)
    : Node(parent)
    , mColor(1.0f, 1.0f, 1.0f, 1.0f)
    , mAmbient(1.0f)
    , mDiffuse(1.0f)
    , mSpecular(1.0f)
{}

BSplineCurves3D::Light::~Light() {}