#include "Curve.h"

BSplineCurves3D::Curve::Curve(QObject* parent)
    : QObject(parent)
    , mSelected(false)
    , mDirty(false) {}

BSplineCurves3D::Curve::~Curve() {}