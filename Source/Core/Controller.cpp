#include "Controller.h"

#include "Core/Constants.h"
#include "Core/CurveContainer.h"
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