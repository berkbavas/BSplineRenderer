#include "Window.h"
#include "Controller.h"
#include "qmath.h"

#include <imgui.h>
#include <QDateTime>
#include <QKeyEvent>
#include <QtImGui.h>

BSplineCurves3D::Window::Window(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
    , mMode(Mode::Select)
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setSamples(16);
    format.setSwapInterval(1);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, this, [=]() { update(); });
}

void BSplineCurves3D::Window::initializeGL()
{
    initializeOpenGLFunctions();

    QtImGui::initialize(this);

    mController = new Controller;
    mController->SetWindow(this);
    mController->Init();

    mRendererManager = RendererManager::Instance();
    mCurveManager = CurveManager::Instance();
    mLightManager = LightManager::Instance();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;
}

void BSplineCurves3D::Window::resizeGL(int w, int h)
{
    mController->Resize(w, h);
}

void BSplineCurves3D::Window::paintGL()
{
    mActiveLight = mLightManager->GetActiveLight();
    mRenderPaths = mRendererManager->GetRenderPaths();
    mRenderPipes = mRendererManager->GetRenderPipes();
    mGlobalPipeRadius = mCurveManager->GetGlobalPipeRadius();
    mGlobalPipeSectorCount = mCurveManager->GetGlobalPipeRadius();
    mSelectedCurve = mCurveManager->GetSelectedCurve();
    mSelectedKnotPoint = mCurveManager->GetSelectedKnotPoint();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    mController->Render(ifps);

    mImguiWantCapture = ImGui::GetIO().WantCaptureMouse;

    QtImGui::newFrame();
    ImGui::SetNextWindowSize(ImVec2(420, 820), ImGuiCond_FirstUseEver);
    ImGui::Begin("Controls", NULL, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import"))
                mController->OnAction(Action::ShowImportWindow);

            if (ImGui::MenuItem("Export"))
                mController->OnAction(Action::ShowExportWindow);

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    // Mode
    if (!ImGui::CollapsingHeader("Mode"))
    {
        int mode = (int)mMode;

        ImGui::RadioButton("Select (Z)", &mode, 0);
        ImGui::RadioButton("Add Knot (C)", &mode, 1);

        if (mode != (int)mMode)
        {
            mMode = (Mode)mode;
            mController->OnAction(Action::UpdateMode, mode);
        }
    }

    // Render Settings
    if (!ImGui::CollapsingHeader("Render Settings"))
    {
        if (ImGui::SliderFloat("Pipe Radius (Global)", &mGlobalPipeRadius, 0.001f, 1.0f, "%.3f"))
            mController->OnAction(Action::UpdateGlobalPipeRadius, mGlobalPipeRadius);

        if (ImGui::SliderInt("Pipe Sector Count (Global)", &mGlobalPipeSectorCount, 3, 192))
            mController->OnAction(Action::UpdateGlobalPipeSectorCount, mGlobalPipeSectorCount);

        if (ImGui::Checkbox("Render Paths", &mRenderPaths))
            mController->OnAction(Action::UpdateRenderPaths, mRenderPaths);

        ImGui::SameLine();

        if (ImGui::Checkbox("Render Pipes", &mRenderPipes))
            mController->OnAction(Action::UpdateRenderPipes, mRenderPipes);
    }

    // Light
    if (!ImGui::CollapsingHeader("Light"))
    {
        {
            ImGui::Text("Position:");
            QVector3D position = mActiveLight->Position();
            float x = position.x();
            float y = position.y();
            float z = position.z();
            float r = position.length();
            float theta = qRadiansToDegrees(atan2(z, x));
            float phi = qRadiansToDegrees(atan2(y, sqrt(z * z + x * x)));
            ImGui::SliderFloat("r", &r, 1.0f, 100.0f, "%.1f");
            ImGui::SliderFloat("theta", &theta, -179.9f, 179.9f, "%.1f");
            ImGui::SliderFloat("phi", &phi, -89.9f, 89.9f, "%.1f");

            x = r * cos(qDegreesToRadians(phi)) * cos(qDegreesToRadians(theta));
            y = r * sin(qDegreesToRadians(phi));
            z = r * cos(qDegreesToRadians(phi)) * sin(qDegreesToRadians(theta));

            mActiveLight->SetPosition(QVector3D(x, y, z));
        }

        {
            ImGui::Text("Shading Parameters:");
            float ambient = mActiveLight->GetAmbient();
            float diffuse = mActiveLight->GetDiffuse();
            float specular = mActiveLight->GetSpecular();

            if (ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f, "%.3f"))
                mActiveLight->SetAmbient(ambient);

            if (ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f, "%.3f"))
                mActiveLight->SetDiffuse(diffuse);

            if (ImGui::SliderFloat("Specular", &specular, 0.0f, 1.0f, "%.3f"))
                mActiveLight->SetSpecular(specular);

            float color[4] = { mActiveLight->GetColor().x(), mActiveLight->GetColor().y(), mActiveLight->GetColor().z(), mActiveLight->GetColor().w() };

            if (ImGui::ColorEdit4("Color##Light", (float*)&color))
                mActiveLight->SetColor(QVector4D(color[0], color[1], color[2], color[3]));
        }
    }

    // Curve
    if (!ImGui::CollapsingHeader("Curve"))
    {
        int n = mSelectedCurve ? mSelectedCurve->GetKnotPoints().size() : 0;
        float length = mSelectedCurve ? mSelectedCurve->Length() : 0.0f;
        float radius = mSelectedCurve ? mSelectedCurve->GetRadius() : 0.0f;
        int sectorCount = mSelectedCurve ? mSelectedCurve->GetSectorCount() : 0;

        ImGui::BeginDisabled(!mSelectedCurve);

        ImGui::Text("Knots: %d", n);
        ImGui::Text("Length: %.2f", length);

        if (ImGui::SliderFloat("Pipe Radius", &radius, 0.001f, 1.0f, "%.3f"))
            mController->OnAction(Action::UpdateSelectedCurvePipeRadius, radius);

        if (ImGui::SliderInt("Pipe Sector Count", &sectorCount, 3, 192))
            mController->OnAction(Action::UpdateSelectedCurvePipeSectorCount, sectorCount);

        ImGui::Text("Shading Parameters:");
        float ambient = mSelectedCurve ? mSelectedCurve->GetMaterial().GetAmbient() : 0.0f;
        float diffuse = mSelectedCurve ? mSelectedCurve->GetMaterial().GetDiffuse() : 0.0f;
        float specular = mSelectedCurve ? mSelectedCurve->GetMaterial().GetSpecular() : 0.0f;
        float x = mSelectedCurve ? mSelectedCurve->GetMaterial().GetColor().x() : 0.0f;
        float y = mSelectedCurve ? mSelectedCurve->GetMaterial().GetColor().y() : 0.0f;
        float z = mSelectedCurve ? mSelectedCurve->GetMaterial().GetColor().z() : 0.0f;
        float w = mSelectedCurve ? mSelectedCurve->GetMaterial().GetColor().w() : 0.0f;
        float color[4] = { x, y, z, w };

        ImGui::SliderFloat("Ambient##Curve", &ambient, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Diffuse##Curve", &diffuse, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Specular##Curve", &specular, 0.0f, 1.0f, "%.3f");
        ImGui::ColorEdit4("Color##Curve", (float*)&color);

        if (mSelectedCurve)
        {
            Material material;
            material.SetAmbient(ambient);
            material.SetDiffuse(diffuse);
            material.SetSpecular(specular);
            material.SetColor(QVector4D(color[0], color[1], color[2], color[3]));
            mSelectedCurve->SetMaterial(material);
        }

        if (ImGui::Button("Remove Curve"))
            mController->OnAction(Action::RemoveSelectedCurve);

        ImGui::EndDisabled();
    }

    // Knot
    if (!ImGui::CollapsingHeader("Knot"))
    {
        float x = mSelectedKnotPoint ? mSelectedKnotPoint->GetPosition().x() : 0;
        float y = mSelectedKnotPoint ? mSelectedKnotPoint->GetPosition().y() : 0;
        float z = mSelectedKnotPoint ? mSelectedKnotPoint->GetPosition().z() : 0;
        bool update = false;

        ImGui::BeginDisabled(!mSelectedKnotPoint);

        ImGui::Text("Position:");

        if (ImGui::DragFloat("x##Knot", &x, 0.001f, -1000, 1000, "%.3f"))
            update = true;

        if (ImGui::DragFloat("y##Knot", &y, 0.001f, -1000, 1000, "%.3f"))
            update = true;

        if (ImGui::DragFloat("z##Knot", &z, 0.001f, -1000, 1000, "%.3f"))
            update = true;

        if (update)
        {
            QList<QVariant> list;
            list << x;
            list << y;
            list << z;
            mController->OnAction(Action::UpdateKnotPointPositionFromGui, list);
        }

        if (ImGui::Button("Remove Knot"))
            mController->OnAction(Action::RemoveSelectedKnotPoint);

        ImGui::EndDisabled();
    }

    ImGui::Spacing();

    if (ImGui::Button("Clear Scene"))
        mController->OnAction(Action::ClearScene);

    ImGui::Spacing();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    glViewport(0, 0, width(), height());
    ImGui::Render();
    QtImGui::render();
}

void BSplineCurves3D::Window::keyPressEvent(QKeyEvent* event)
{
    mController->KeyPressed(event);
}

void BSplineCurves3D::Window::keyReleaseEvent(QKeyEvent* event)
{
    mController->KeyReleased(event);
}

void BSplineCurves3D::Window::mousePressEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    mController->MousePressed(event);
}

void BSplineCurves3D::Window::mouseReleaseEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    mController->MouseReleased(event);
}

void BSplineCurves3D::Window::mouseMoveEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    mController->MouseMoved(event);
}

void BSplineCurves3D::Window::wheelEvent(QWheelEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    mController->WheelMoved(event);
}

void BSplineCurves3D::Window::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    mController->MouseDoubleClicked(event);
}

void BSplineCurves3D::Window::OnModeChanged(Mode newMode)
{
    mMode = newMode;
}