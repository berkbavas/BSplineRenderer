#include "EventHandler.h"

#include "Core/CurveContainer.h"
#include "Renderer/RendererManager.h"
#include "Util/Logger.h"

BSplineRenderer::EventHandler::EventHandler(QObject* parent)
    : QObject(parent)
{
}

void BSplineRenderer::EventHandler::OnKeyPressed(QKeyEvent* event)
{
    mCamera->KeyPressed(event);
}

void BSplineRenderer::EventHandler::OnKeyReleased(QKeyEvent* event)
{
    mCamera->KeyReleased(event);
}

void BSplineRenderer::EventHandler::OnMousePressed(QMouseEvent* event)
{
    mCamera->MousePressed(event);
    mMouse.button = event->button();
    mMouse.x = event->x() * mDevicePixelRatio;
    mMouse.y = event->y() * mDevicePixelRatio;

    if (event->button() == Qt::LeftButton)
    {
        TrySelectCurve(mMouse.x, mMouse.y);

        if (mSelectedCurve)
        {
            TrySelectKnot(mMouse.x, mMouse.y);
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        Eigen::Vector3f normal = GetCameraViewDirection<Eigen::Vector3f>();
        Eigen::Vector3f origin = GetCameraPosition<Eigen::Vector3f>();
        Eigen::ParametrizedLine<float, 3> ray = GetRayFromScreenCoordinates(mMouse.x, mMouse.y);

        if (mSelectedCurve)
        {
            float x = mSelectedCurve->GetKnots().last()->GetPosition().x();
            float y = mSelectedCurve->GetKnots().last()->GetPosition().y();
            float z = mSelectedCurve->GetKnots().last()->GetPosition().z();
            Eigen::Vector3f lastKnotPosition = Eigen::Vector3f(x, y, z);

            Eigen::Hyperplane<float, 3> plane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(lastKnotPosition));
            const float t = ray.intersection(plane);
            Eigen::Vector3f intersection = ray.pointAt(t);

            if (std::isnan(t) == false && std::isinf(t) == false)
            {
                const auto knot = mSelectedCurve->AddKnot(intersection.x(), intersection.y(), intersection.z());
                SetSelectedKnot(knot);
            }
            else
            {
                LOG_DEBUG("EventHandler::OnMousePressed: There is no intersection between ray along the view direction and translation plane.");
            }
        }
        else
        {
            Eigen::Hyperplane<float, 3> plane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(origin + 20 * normal));
            float t = ray.intersection(plane);
            Eigen::Vector3f intersection = ray.pointAt(t);

            if (std::isnan(t) == false && std::isinf(t) == false)
            {
                SplinePtr spline = std::make_shared<Spline>();
                const auto knot = spline->AddKnot(intersection.x(), intersection.y(), intersection.z());
                mCurveContainer->AddCurve(spline);
                SetSelectedCurve(spline);
                SetSelectedKnot(knot);
            }
            else
            {
                LOG_DEBUG("EventHandler::OnMousePressed: There is no intersection between ray along the view direction and translation plane.");
            }
        }
    }
}

void BSplineRenderer::EventHandler::OnMouseReleased(QMouseEvent* event)
{
    mCamera->MouseReleased(event);

    mMouse.button = Qt::NoButton;
}

void BSplineRenderer::EventHandler::OnMouseMoved(QMouseEvent* event)
{
    mCamera->MouseMoved(event);

    mMouse.x = event->x() * mDevicePixelRatio;
    mMouse.y = event->y() * mDevicePixelRatio;

    if (mMouse.button == Qt::LeftButton)
    {
        if (mSelectedKnot)
        {
            Eigen::ParametrizedLine<float, 3> line = GetRayFromScreenCoordinates(mMouse.x, mMouse.y);
            float t = line.intersection(mKnotTranslationPlane);
            Eigen::Vector3f intersection = line.pointAt(t);

            if (std::isnan(t) == false && std::isinf(t) == false)
            {
                mSelectedKnot->SetPosition(intersection.x(), intersection.y(), intersection.z());

                if (mSelectedCurve)
                {
                    mSelectedCurve->MakeDirty();
                }
            }
        }
    }
}

void BSplineRenderer::EventHandler::OnWheelMoved(QWheelEvent* event)
{
}

Eigen::Vector3f BSplineRenderer::EventHandler::GetViewDirection() const
{
    const auto& direction = mCamera->GetViewDirection();
    return Eigen::Vector3f(direction.x(), direction.y(), direction.z()).normalized();
}

void BSplineRenderer::EventHandler::SetSelectedKnot(KnotPtr knot)
{
    if (mSelectedKnot == knot)
        return;

    mSelectedKnot = knot;
    emit SelectedKnotChanged(mSelectedKnot);
}

void BSplineRenderer::EventHandler::SetSelectedCurve(SplinePtr spline)
{
    if (mSelectedCurve == spline)
        return;

    SetSelectedKnot(nullptr);
    mSelectedCurve = spline;
    emit SelectedCurveChanged(mSelectedCurve);
}

void BSplineRenderer::EventHandler::TrySelectKnot(float x, float y)
{
    KnotPtr selectedKnot = nullptr;

    if (mSelectedCurve)
    {
        QVector3D direction = mCamera->GetDirectionFromScreenCoodinates(x, y);
        QVector3D origin = mCamera->GetPosition();
        selectedKnot = mSelectedCurve->GetClosestKnotToRay(origin, direction, 2.0f * mSelectedCurve->GetRadius());
    }

    SetSelectedKnot(selectedKnot);
    UpdateKnotTranslationPlane();
}

void BSplineRenderer::EventHandler::TrySelectCurve(float x, float y)
{
    CurveQueryInfo info = mRendererManager->Query(QPoint(x, y));

    if (info.result == 1)
    {
        SetSelectedCurve(mCurveContainer->GetCurve(info.index));
    }
    else
    {
        SetSelectedCurve(nullptr);
    }
}

void BSplineRenderer::EventHandler::UpdateKnotTranslationPlane()
{
    if (mSelectedKnot)
    {
        const float x = mSelectedKnot->GetPosition().x();
        const float y = mSelectedKnot->GetPosition().y();
        const float z = mSelectedKnot->GetPosition().z();
        Eigen::Vector3f knotPosition = Eigen::Vector3f(x, y, z);
        Eigen::Vector3f normal = GetCameraViewDirection<Eigen::Vector3f>();
        mKnotTranslationPlane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(knotPosition));
    }
}

Eigen::ParametrizedLine<float, 3> BSplineRenderer::EventHandler::GetRayFromScreenCoordinates(float x, float y)
{
    Eigen::Vector3f direction = GetDirectionFromScreenCoodinates<Eigen::Vector3f>(x, y);
    Eigen::Vector3f origin = GetCameraPosition<Eigen::Vector3f>();

    return Eigen::ParametrizedLine<float, 3>(origin, direction);
}
