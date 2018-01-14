#include "mainwindow.h"
#include <QApplication>

#include "test.cpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setBlueBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);


    Test test;

    test.LoadFromFBX("://9v.fbx");

    MainWindow w;
    w.show();

    return a.exec();
}
