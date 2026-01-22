#include "Controller.h"

#include "Core/AnimationManager.h"
#include "Core/Constants.h"
#include "Core/CurveContainer.h"
#include "Core/UndoRedoManager.h"
#include "Core/Window.h"
#include "Gui/ImGuiWindow.h"
#include "Renderer/RendererManager.h"
#include "Util/Logger.h"

#include <QThread>
#include <QtImGui.h>
#include <imgui.h>

BSplineRenderer::Controller::Controller(QObject* parent)
    : QObject(parent)
{
    mWindow = new Window;
    mEventHandler = new EventHandler(this);
    mImGuiWindow = new ImGuiWindow(this);
    mRendererManager = new RendererManager;
    mCurveContainer = new CurveContainer;

    mCamera = mRendererManager->GetCamera();
    mEventHandler->SetCamera(mCamera);
    mEventHandler->SetCurveContainer(mCurveContainer);
    mEventHandler->SetRendererManager(mRendererManager);

    mRendererManager->SetCurveContainer(mCurveContainer);
    mImGuiWindow->SetRendererManager(mRendererManager);
    mImGuiWindow->SetCurveContainer(mCurveContainer);

    connect(mWindow, &Window::Initialize, this, &Controller::Initialize);
    connect(mWindow, &Window::Render, this, &Controller::Render);
    connect(mWindow, &Window::Resize, this, &Controller::Resize);
    connect(mWindow, &Window::MousePressed, this, &Controller::OnMousePressed);
    connect(mWindow, &Window::MouseReleased, this, &Controller::OnMouseReleased);
    connect(mWindow, &Window::MouseMoved, this, &Controller::OnMouseMoved);
    connect(mWindow, &Window::WheelMoved, this, &Controller::OnWheelMoved);
    connect(mWindow, &Window::KeyPressed, this, &Controller::OnKeyPressed);
    connect(mWindow, &Window::KeyReleased, this, &Controller::OnKeyReleased);

    connect(mEventHandler, &EventHandler::SelectedCurveChanged, this, [this](SplinePtr curve)
            {
                mRendererManager->SetSelectedCurve(curve);
                mImGuiWindow->SetSelectedCurve(curve); //
            });

    connect(mEventHandler, &EventHandler::SelectedKnotChanged, this, [this](KnotPtr knot)
            {
                mRendererManager->SetSelectedKnot(knot);
                mImGuiWindow->SetSelectedKnot(knot); //
            });

    connect(mEventHandler, &EventHandler::KnotAroundChanged, this, [this](KnotPtr knot)
            { mRendererManager->SetKnotAround(knot); });

    // Connect ImGuiWindow signals
    connect(mImGuiWindow, &ImGuiWindow::RequestCameraReset, this, [this]()
            { mCamera->Reset(); });

    connect(mImGuiWindow, &ImGuiWindow::RequestCameraPreset, this, [this](int preset)
            { ApplyCameraPreset(preset); });

    connect(mImGuiWindow, &ImGuiWindow::CurveAdded, this, [this](SplinePtr spline)
            {
                mEventHandler->SetSelectedCurve(spline);
                LOG_INFO("Preset shape added with {} knots", spline->GetKnotCount());
            });
}

BSplineRenderer::Controller::~Controller()
{
    qDebug() << "Controller::~Controller: Application closing...";
    qDebug() << "Controller::~Controller: Current Thread:" << QThread::currentThread();
}

void BSplineRenderer::Controller::Run()
{
    qDebug() << "Controller::Controller: Application starting...";

    mWindow->resize(mWidth, mHeight);
    mWindow->show();
}

void BSplineRenderer::Controller::Initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mRendererManager->Initialize();

    QtImGui::initialize(mWindow);
}

void BSplineRenderer::Controller::Render(float ifps)
{
    mCamera->Resize(mWidth, mHeight);
    mCamera->Update(ifps);
    mEventHandler->SetDevicePixelRatio(mDevicePixelRatio);

    // Update animations
    AnimationManager::Instance().Update(ifps, mCurveContainer);

    mRendererManager->Render();

    QtImGui::newFrame();

    mImGuiWindow->Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mWidth, mHeight);

    ImGui::Render();
    QtImGui::render();
}

void BSplineRenderer::Controller::OnKeyPressed(QKeyEvent* event)
{
    mEventHandler->OnKeyPressed(event);
}

void BSplineRenderer::Controller::OnKeyReleased(QKeyEvent* event)
{
    mEventHandler->OnKeyReleased(event);
}

void BSplineRenderer::Controller::Resize(int width, int height)
{
    mDevicePixelRatio = mWindow->devicePixelRatio();
    mWidth = width * mDevicePixelRatio;
    mHeight = height * mDevicePixelRatio;

    mWindow->makeCurrent();
    mRendererManager->Resize(width, height);
    mWindow->doneCurrent();
}

void BSplineRenderer::Controller::OnMousePressed(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->OnMousePressed(event);
}

void BSplineRenderer::Controller::OnMouseReleased(QMouseEvent* event)
{
    mEventHandler->OnMouseReleased(event);
}

void BSplineRenderer::Controller::OnMouseMoved(QMouseEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->OnMouseMoved(event);
}

void BSplineRenderer::Controller::OnWheelMoved(QWheelEvent* event)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    mEventHandler->OnWheelMoved(event);
}

void BSplineRenderer::Controller::ApplyCameraPreset(int preset)
{
    // Camera preset positions
    switch (preset)
    {
    case 0: // Front
        mCamera->SetPosition(QVector3D(0, 0, 30));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(0, 0, 0));
        break;
    case 1: // Back
        mCamera->SetPosition(QVector3D(0, 0, -30));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(0, 180, 0));
        break;
    case 2: // Left
        mCamera->SetPosition(QVector3D(-30, 0, 0));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(0, 90, 0));
        break;
    case 3: // Right
        mCamera->SetPosition(QVector3D(30, 0, 0));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(0, -90, 0));
        break;
    case 4: // Top
        mCamera->SetPosition(QVector3D(0, 30, 0));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(90, 0, 0));
        break;
    case 5: // Bottom
        mCamera->SetPosition(QVector3D(0, -30, 0));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(-90, 0, 0));
        break;
    case 6: // Isometric
        mCamera->SetPosition(QVector3D(20, 20, 20));
        mCamera->SetRotation(QQuaternion::fromEulerAngles(35.264f, -45, 0));
        break;
    }
}