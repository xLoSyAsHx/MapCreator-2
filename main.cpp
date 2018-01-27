#include "mainwindow.h"
#include <QApplication>

#include "ModelLoader.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setBlueBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    //Model3D* model = ModelLoader::loadFromFile("G:\\Programming\\Qt\\MapCreator\\9v.fbx");

    MainWindow w;
    w.show();

    return a.exec();
}
