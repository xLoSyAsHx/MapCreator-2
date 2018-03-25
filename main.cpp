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

    LandscapeSculptTool::Instance().setCenter(QVector2D(-5, -5));
    LandscapeSculptTool::Instance().setRadius(3);
    LandscapeSculptTool::Instance().setBryshFalloff(0.5f);
    LandscapeSculptTool::Instance().setToolStrength(1.0f);


    Landscape l(10, 10, 1);
    l.refreshByLandscapeTool();

    MainWindow w;
    w.show();

    return a.exec();
}
