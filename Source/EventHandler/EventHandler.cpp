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
        TrySelectCurve(event->position().toPoint());
    }
    else if (event->button() == Qt::RightButton)
    {
        QVector3D rayDirection = mCamera->GetDirectionFromScreenCoodinates(mMouse.x, mMouse.y);
        QVector3D rayOrigin = mCamera->GetPosition();
        QVector3D viewDirection = mCamera->GetViewDirection();

        Eigen::Vector3f direction = Eigen::Vector3f(rayDirection.x(), rayDirection.y(), rayDirection.z());
        Eigen::Vector3f origin = Eigen::Vector3f(rayOrigin.x(), rayOrigin.y(), rayOrigin.z());
        Eigen::Vector3f normal = Eigen::Vector3f(viewDirection.x(), viewDirection.y(), viewDirection.z());

        const auto ray = Eigen::ParametrizedLine<float, 3>(origin, direction);

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
    emit SelectedKnotChanged(knot);
}

void BSplineRenderer::EventHandler::SetSelectedCurve(SplinePtr spline)
{
    if (mSelectedCurve == spline)
        return;

    SetSelectedKnot(nullptr);
    mSelectedCurve = spline;
    emit SelectedCurveChanged(spline);
}

void BSplineRenderer::EventHandler::TrySelectCurve(const QPoint& mousePosition)
{
    CurveQueryInfo info = mRendererManager->Query(mousePosition);

    if (info.result == 1)
    {
        SetSelectedCurve(mCurveContainer->GetCurve(info.index));
    }
    else
    {
        SetSelectedCurve(nullptr);
    }
}
