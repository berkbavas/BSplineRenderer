#include "Controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{}

void Controller::init()
{
    mProjection.perspective(60, 1, 0.1, 10000);

    mCamera = new Camera;
    mCamera->setPosition(QVector3D(0, 20, 0));
    mCamera->setProjection(mProjection);

    mLight = new Light;
    mLight->setPosition(QVector3D(5, 20, 35));

    mRendererManager = new RendererManager;
    mRendererManager->setCamera(mCamera);
    mRendererManager->setLight(mLight);

    mWindow = new Window;
    mWindow->setRendererManager(mRendererManager);
    mWindow->resize(800, 600);

    connect(mWindow, &Window::wheelMoved, this, &Controller::onWheelMoved);
    connect(mWindow, &Window::mousePressed, this, &Controller::onMousePressed);
    connect(mWindow, &Window::mouseReleased, this, &Controller::onMouseReleased);
    connect(mWindow, &Window::mouseMoved, this, &Controller::onMouseMoved);
    connect(mWindow, &Window::keyPressed, this, &Controller::onKeyPressed);
    connect(mWindow, &Window::keyReleased, this, &Controller::onKeyReleased);
    connect(mWindow, &Window::resizeReceived, this, &Controller::onResizeReceived);
}

void Controller::show()
{
    mWindow->show();
}

void Controller::onWheelMoved(QWheelEvent *event) {}

void Controller::onMousePressed(QMouseEvent *event)
{
    mCamera->onMousePressed(event);
}

void Controller::onMouseReleased(QMouseEvent *event)
{
    mCamera->onMouseReleased(event);
}

void Controller::onMouseMoved(QMouseEvent *event)
{
    mCamera->onMouseMoved(event);
}

void Controller::onKeyPressed(QKeyEvent *event)
{
    mCamera->onKeyPressed(event);
}

void Controller::onKeyReleased(QKeyEvent *event)
{
    mCamera->onKeyReleased(event);
}

void Controller::onResizeReceived(int w, int h)
{
    mProjection.setToIdentity();
    mProjection.perspective(60, (float) (w) / h, 0.1, 10000);
    mCamera->setProjection(mProjection);
}
