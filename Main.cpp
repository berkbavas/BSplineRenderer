#include "Controller.h"
#include "Window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller controller;
    controller.init();
    controller.show();

    return a.exec();
}
