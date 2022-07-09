#include "Window.h"

#include <imgui.h>
#include <QKeyEvent>
#include <QtImGui.h>

Window::Window(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
{
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setSamples(16);
    format.setSwapInterval(1);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, this, [=]() { update(); });
}

Window::~Window() {}

void Window::initializeGL()
{
    initializeOpenGLFunctions();
    QtImGui::initialize(this);

    mRendererManager->init();
}

void Window::resizeGL(int w, int h)
{
    emit resizeReceived(w, h);
}

void Window::paintGL()
{
    mRendererManager->render();

    QtImGui::newFrame();

    ImGui::Text("Does ImGui work?");

    glViewport(0, 0, width(), height());
    ImGui::Render();
    QtImGui::render();
}

void Window::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    emit keyReleased(event);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(event);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event);
}

void Window::wheelEvent(QWheelEvent *event)
{
    emit wheelMoved(event);
}

void Window::setRendererManager(RendererManager *newRendererManager)
{
    mRendererManager = newRendererManager;
}
