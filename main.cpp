#include "mainwindow.h"
#include <QApplication>

#include <landscape.h>

#define TEST_MODE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setBlueBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    //Model3D* model = ModelLoader::loadFromFile("G:\\Programming\\Qt\\MapCreator\\9v.fbx");

    Landscape l(8, 8, 1);
    Border2D b =  l.refreshByLandscapeTool(QVector2D(0, 0), 1);

    MainWindow w;
    w.show();

    return a.exec();
}
