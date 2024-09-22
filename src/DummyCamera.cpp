#include "DummyCamera.h"

BSplineCurves3D::DummyCamera::DummyCamera(QObject* parent)
    : Camera(parent)
{}

BSplineCurves3D::DummyCamera::~DummyCamera() {}

void BSplineCurves3D::DummyCamera::KeyPressed(QKeyEvent*) {}

void BSplineCurves3D::DummyCamera::KeyReleased(QKeyEvent*) {}

void BSplineCurves3D::DummyCamera::MousePressed(QMouseEvent*) {}

void BSplineCurves3D::DummyCamera::MouseReleased(QMouseEvent*) {}

void BSplineCurves3D::DummyCamera::MouseMoved(QMouseEvent*) {}

void BSplineCurves3D::DummyCamera::Update(float) {}