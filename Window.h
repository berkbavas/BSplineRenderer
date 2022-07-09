#ifndef WINDOW_H
#define WINDOW_H

#include "RendererManager.h"

#include <QOpenGLFunctions>
#include <QOpenGLWindow>

class Window : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Window(QWindow *parent = nullptr);
    ~Window();

    void setRendererManager(RendererManager *newRendererManager);

signals:
    void wheelMoved(QWheelEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);
    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void resizeReceived(int w, int h);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;
    virtual void wheelEvent(QWheelEvent *) override;

private:
    RendererManager *mRendererManager;
};
#endif // WINDOW_H
