#include "Controller.h"
#include "FreeCamera.h"
#include "Helper.h"
#include "Light.h"
#include "Window.h"

#include <QDebug>

BSplineCurves3D::Controller::Controller(QObject* parent)
    : QObject(parent)
    , mSelectedCurve(nullptr)
    , mSelectedKnotPoint(nullptr)
    , mPressedButton(Qt::NoButton)
    , mMode(Mode::Select)
{}

void BSplineCurves3D::Controller::Init()
{
    mRendererManager = RendererManager::Instance();
    mCameraManager = CameraManager::Instance();
    mLightManager = LightManager::Instance();
    mModelManager = ModelManager::Instance();
    mCurveManager = CurveManager::Instance();

    mRendererManager->Init();

    mFileDialog = new QFileDialog;

    connect(mCurveManager, &CurveManager::SelectedCurveChanged, this, [=](Spline* selectedCurve) { mSelectedCurve = selectedCurve; });
    connect(mCurveManager, &CurveManager::SelectedKnotPointChanged, this, [=](KnotPoint* selectedPoint) { mSelectedKnotPoint = selectedPoint; });

    connect(this, &BSplineCurves3D::Controller::ModeChanged, mWindow, &Window::OnModeChanged);

    connect(mFileDialog, &QFileDialog::fileSelected, this, [=](const QString& path) {
        if (!path.isEmpty())
            switch (mLastFileAction)
            {
            case Action::ShowExportWindow:
                OnAction(Action::Export, path);
                break;
            case Action::ShowImportWindow:
                OnAction(Action::Import, path);
                break;
            default:
                break;
            }
        });

    mCamera = new FreeCamera;
    mCamera->SetPosition(QVector3D(0, 10, 10));
    mCamera->SetVerticalFov(60.0f);
    mCamera->SetZNear(0.1f);
    mCamera->SetZFar(10000.0f);
    mCameraManager->AddCamera(mCamera);
    mCameraManager->SetActiveCamera(mCamera);

    mLight = new Light;
    mLight->SetPosition(QVector3D(5, 20, 35));
    mLightManager->AddLight(mLight);
    mLightManager->SetActiveLight(mLight);

    // Plane
    {
        mPlane = new Model;
        mPlane->SetType(Model::Plane);
        mPlane->setObjectName("Plane");
        mPlane->SetPosition(QVector3D(0, 0, 0));
        mPlane->SetVisible(true);
        mModelManager->AddModel(mPlane);
    }

    mCurveManager->AddCurves(Helper::LoadCurveDataFromJson(":/Resources/Data/test-curves-light.json"));
}

void BSplineCurves3D::Controller::OnAction(Action action, QVariant variant)
{
    switch (action)
    {
    case Action::Select: {
        QVector3D rayDirection = mCameraManager->GetDirectionFromScreen(variant.toPoint().x(), variant.toPoint().y(), mWindow->width(), mWindow->height());
        QVector3D rayOrigin = mCameraManager->GetActiveCamera()->Position();

        if (mSelectedCurve)
            mCurveManager->SelectKnotPoint(rayOrigin, rayDirection);

        if (!mSelectedKnotPoint)
            mCurveManager->SelectCurve(rayOrigin, rayDirection);

        if (mSelectedKnotPoint)
        {
            float x = mSelectedKnotPoint->GetPosition().x();
            float y = mSelectedKnotPoint->GetPosition().y();
            float z = mSelectedKnotPoint->GetPosition().z();

            QVector3D viewDirection = mCameraManager->GetActiveCamera()->GetViewDirection();
            Eigen::Vector3f normal = Eigen::Vector3f(viewDirection.x(), viewDirection.y(), viewDirection.z());
            Eigen::Vector3f eigenControlPointPosition = Eigen::Vector3f(x, y, z);
            normal.normalize();
            mTranslationPlane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(eigenControlPointPosition));
        }

        break;
    }
    case Action::AddKnot: {
        QVector3D rayDirection = mCameraManager->GetDirectionFromScreen(variant.toPoint().x(), variant.toPoint().y(), mWindow->width(), mWindow->height());
        QVector3D rayOrigin = mCameraManager->GetActiveCamera()->Position();
        QVector3D viewDirection = mCameraManager->GetActiveCamera()->GetViewDirection();

        Eigen::Vector3f eigenRayDirection = Eigen::Vector3f(rayDirection.x(), rayDirection.y(), rayDirection.z());
        Eigen::Vector3f eigenRayOrigin = Eigen::Vector3f(rayOrigin.x(), rayOrigin.y(), rayOrigin.z());
        Eigen::Vector3f normal = Eigen::Vector3f(viewDirection.x(), viewDirection.y(), viewDirection.z()).normalized();

        auto line = Eigen::ParametrizedLine<float, 3>(eigenRayOrigin, eigenRayDirection);

        if (mSelectedCurve)
        {
            float x = mSelectedCurve->GetKnotPoints().last()->GetPosition().x();
            float y = mSelectedCurve->GetKnotPoints().last()->GetPosition().y();
            float z = mSelectedCurve->GetKnotPoints().last()->GetPosition().z();
            Eigen::Vector3f lastKnotPosition = Eigen::Vector3f(x, y, z);

            Eigen::Hyperplane<float, 3> plane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(lastKnotPosition));
            float t = line.intersection(plane);
            Eigen::Vector3f intersection = line.pointAt(t);

            if (!isnan(t) && !isinf(t))
            {
                KnotPoint* point = new KnotPoint(intersection.x(), intersection.y(), intersection.z());
                mSelectedCurve->AddKnotPoint(point);
                mCurveManager->SetSelectedKnotPoint(point);
            }
        }
        else
        {
            Eigen::Hyperplane<float, 3> plane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(eigenRayOrigin + 20 * normal));
            float t = line.intersection(plane);
            Eigen::Vector3f intersection = line.pointAt(t);

            if (!isnan(t) && !isinf(t))
            {
                Spline* spline = new Spline;
                KnotPoint* point = new KnotPoint(intersection.x(), intersection.y(), intersection.z());
                spline->AddKnotPoint(point);
                mCurveManager->AddCurve(spline);
                mCurveManager->SetSelectedCurve(spline);
                mCurveManager->SetSelectedKnotPoint(point);
            }
        }

        break;
    }
    case Action::UpdateMode: {
        mMode = (Mode)variant.toInt();

        if (mSelectedKnotPoint)
        {
            float x = mSelectedKnotPoint->GetPosition().x();
            float y = mSelectedKnotPoint->GetPosition().y();
            float z = mSelectedKnotPoint->GetPosition().z();

            QVector3D viewDirection = mCameraManager->GetActiveCamera()->GetViewDirection();
            Eigen::Vector3f normal = Eigen::Vector3f(viewDirection.x(), viewDirection.y(), viewDirection.z()).normalized();
            Eigen::Vector3f knotPosition = Eigen::Vector3f(x, y, z);
            mTranslationPlane = Eigen::Hyperplane<float, 3>(normal, -normal.dot(knotPosition));
        }

        if (mMode == Mode::Select)
        {
            mCurveManager->SetSelectedCurve(nullptr);
        }

        emit ModeChanged(mMode);

        break;
    }
    case Action::UpdateRenderPaths: {
        mRendererManager->SetRenderPaths(variant.toBool());
        break;
    }
    case Action::UpdateRenderPipes: {
        mRendererManager->SetRenderPipes(variant.toBool());
        break;
    }
    case Action::UpdateKnotPointPositionFromScreen: {
        if (mSelectedKnotPoint)
        {
            QVector3D rayDirection = mCameraManager->GetDirectionFromScreen(variant.toPoint().x(), variant.toPoint().y(), mWindow->width(), mWindow->height());
            QVector3D rayOrigin = mCamera->Position();

            Eigen::Vector3f eigenRayDirection = Eigen::Vector3f(rayDirection.x(), rayDirection.y(), rayDirection.z());
            Eigen::Vector3f eigenRayOrigin = Eigen::Vector3f(rayOrigin.x(), rayOrigin.y(), rayOrigin.z());

            auto line = Eigen::ParametrizedLine<float, 3>(eigenRayOrigin, eigenRayDirection);
            float t = line.intersection(mTranslationPlane);
            Eigen::Vector3f intersection = line.pointAt(t);

            if (!isnan(t) && !isinf(t))
            {
                mSelectedKnotPoint->SetPosition(QVector3D(intersection.x(), intersection.y(), intersection.z()));
            }
        }
        break;
    }
    case Action::UpdateKnotPointPositionFromGui: {
        if (mSelectedKnotPoint)
        {
            QList<QVariant> coords = variant.toList();
            float x = coords[0].toFloat();
            float y = coords[1].toFloat();
            float z = coords[2].toFloat();
            mSelectedKnotPoint->SetPosition(QVector3D(x, y, z));
        }
        break;
    }
    case Action::UpdateSelectedCurvePipeRadius: {
        if (mSelectedCurve)
            mSelectedCurve->SetRadius(variant.toFloat());
        break;
    }
    case Action::UpdateSelectedCurvePipeSectorCount: {
        if (mSelectedCurve)
            mSelectedCurve->SetSectorCount(variant.toInt());
        break;
    }
    case Action::UpdateGlobalPipeRadius: {
        mCurveManager->SetGlobalPipeRadius(variant.toFloat());
        break;
    }
    case Action::UpdateGlobalPipeSectorCount: {
        mCurveManager->SetGlobalPipeSectorCount(variant.toInt());
        break;
    }
    case Action::RemoveSelectedCurve: {
        mCurveManager->RemoveCurve(mSelectedCurve);
        break;
    }
    case Action::RemoveSelectedKnotPoint: {
        if (mSelectedCurve)
        {
            if (mSelectedKnotPoint)
            {
                mSelectedCurve->RemoveKnotPoint(mSelectedKnotPoint);
                mCurveManager->SetSelectedKnotPoint(nullptr);
            }
        }
        break;
    }
    case Action::ClearScene: {
        mCurveManager->RemoveAllCurves();
        break;
    }
    case Action::ShowImportWindow: {
        mLastFileAction = Action::ShowImportWindow;
        mFileDialog->setFileMode(QFileDialog::ExistingFile);
        mFileDialog->setAcceptMode(QFileDialog::AcceptOpen);
        mFileDialog->setNameFilter("*.json");
        mFileDialog->show();
        break;
    }
    case Action::ShowExportWindow: {
        mLastFileAction = Action::ShowExportWindow;
        mFileDialog->setFileMode(QFileDialog::AnyFile);
        mFileDialog->setAcceptMode(QFileDialog::AcceptSave);
        mFileDialog->setDefaultSuffix(".json");
        mFileDialog->setNameFilter("*.json");
        mFileDialog->show();
        break;
    }
    case Action::Export: {
        Helper::SaveCurveDataToJson(mCurveManager->GetCurvesNonConst(), variant.toString());
        break;
    }
    case Action::Import: {
        QList<Spline*> curves = Helper::LoadCurveDataFromJson(variant.toString());

        if (!curves.isEmpty())
        {
            mCurveManager->SetSelectedCurve(nullptr);
            mCurveManager->RemoveAllCurves();
            mCurveManager->AddCurves(curves);
        }
        break;
    }
    }
}

void BSplineCurves3D::Controller::SetWindow(Window* newWindow)
{
    mWindow = newWindow;
}

void BSplineCurves3D::Controller::WheelMoved(QWheelEvent*)
{
}

void BSplineCurves3D::Controller::MousePressed(QMouseEvent* event)
{
    mPressedButton = event->button();

    if (mPressedButton == Qt::RightButton)
    {
        mCameraManager->MousePressed(event);
    }
    else if (mPressedButton == Qt::LeftButton)
    {
        switch (mMode)
        {
        case Mode::Select: {
            OnAction(Action::Select, event->pos());
            break;
        }
        case Mode::Add: {
            OnAction(Action::AddKnot, event->pos());
            break;
        }
        }
    }
}

void BSplineCurves3D::Controller::MouseReleased(QMouseEvent* event)
{
    mPressedButton = Qt::NoButton;

    if (event->button() == Qt::RightButton)
    {
        mCameraManager->MouseReleased(event);
    }
}

void BSplineCurves3D::Controller::MouseMoved(QMouseEvent* event)
{
    if (mPressedButton == Qt::RightButton)
    {
        mCameraManager->MouseMoved(event);
    }
    else if (mPressedButton == Qt::LeftButton)
    {
        switch (mMode)
        {
        case Mode::Select: {
            OnAction(Action::UpdateKnotPointPositionFromScreen, event->pos());
            break;
        }
        case Mode::Add: {
            break;
        }
        }
    }
}

void BSplineCurves3D::Controller::KeyPressed(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
    {
        if (mSelectedKnotPoint)
            OnAction(Action::RemoveSelectedKnotPoint);
        else if (mSelectedCurve)
            OnAction(Action::RemoveSelectedCurve);
    }
    else if (event->key() == Qt::Key_Z)
    {
        OnAction(Action::UpdateMode, (int)Mode::Select);
    }
    else if (event->key() == Qt::Key_C)
    {
        OnAction(Action::UpdateMode, (int)Mode::Add);
    }

    mCameraManager->KeyPressed(event);
}

void BSplineCurves3D::Controller::KeyReleased(QKeyEvent* event)
{
    mCameraManager->KeyReleased(event);
}

void BSplineCurves3D::Controller::Resize(int w, int h)
{
    mCamera->SetAspectRatio((float)(w) / h);
}

void BSplineCurves3D::Controller::MouseDoubleClicked(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        OnAction(Action::UpdateMode, (int)Mode::Select);
}

void BSplineCurves3D::Controller::Render(float ifps)
{
    mCameraManager->Update(ifps);
    mRendererManager->Render(ifps);
}