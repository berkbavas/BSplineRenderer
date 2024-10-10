#include "Window.h"

#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>

BSplineRenderer::Window::Window(QWindow* parent)
    : QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)

{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(8);
    setFormat(format);

    connect(this, &QOpenGLWindow::frameSwapped, [=]()
            { update(); });
}

void BSplineRenderer::Window::initializeGL()
{
    initializeOpenGLFunctions();

    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    mPreviousTime = mCurrentTime;

    emit Initialize();
}

void BSplineRenderer::Window::resizeGL(int width, int height)
{
    emit Resize(width, height);
}

void BSplineRenderer::Window::paintGL()
{
    mCurrentTime = QDateTime::currentMSecsSinceEpoch();
    const float ifps = (mCurrentTime - mPreviousTime) * 0.001f;
    mPreviousTime = mCurrentTime;

    emit Render(ifps);
}

void BSplineRenderer::Window::keyPressEvent(QKeyEvent* event)
{
    emit KeyPressed(event);
}

void BSplineRenderer::Window::keyReleaseEvent(QKeyEvent* event)
{
    emit KeyReleased(event);
}

void BSplineRenderer::Window::mousePressEvent(QMouseEvent* event)
{
    emit MousePressed(event);
}

void BSplineRenderer::Window::mouseReleaseEvent(QMouseEvent* event)
{
    emit MouseReleased(event);
}

void BSplineRenderer::Window::mouseMoveEvent(QMouseEvent* event)
{
    emit MouseMoved(event);
}

void BSplineRenderer::Window::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}
