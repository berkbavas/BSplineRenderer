#include "Material.h"

BSplineCurves3D::Material::Material()
    : mAmbient(0.25f)
    , mDiffuse(0.75f)
    , mSpecular(0.25f)
    , mShininess(32.0f)
    , mColor(1, 1, 1, 1)
{}