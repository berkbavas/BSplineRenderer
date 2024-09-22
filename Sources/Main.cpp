#include "Window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BSplineCurves3D::Window w;
    w.resize(1600, 900);
    w.show();

    return a.exec();
}
