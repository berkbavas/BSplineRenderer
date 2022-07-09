#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Camera.h"
#include "Window.h"

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    void init();
    void show();

protected:
    void onWheelMoved(QWheelEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(QMouseEvent *event);
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onResizeReceived(int w, int h);

private:
    QMatrix4x4 mProjection;
    Camera *mCamera;
    Light *mLight;
    RendererManager *mRendererManager;
    Window *mWindow;
};

#endif // CONTROLLER_H
