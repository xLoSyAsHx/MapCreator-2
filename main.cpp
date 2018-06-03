#include "mainwindow.h"
#include <QApplication>

#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>

#include <QDockWidget>
#include <QMainWindow>

#include <landscape.h>


#include <QListWidget>
#include <QListWidgetItem>


#include "mainapplicationwindow.h"
#define TEST_MODE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setBlueBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    //Model3D* model = ModelLoader::loadFromFile("G:\\Programming\\Qt\\MapCreator\\9v.fbx");

    LandscapeSculptTool::Instance().setCenter(QVector2D(50, 50));
    LandscapeSculptTool::Instance().setRadius(5);
    LandscapeSculptTool::Instance().setBryshFalloff(0.5f);
    LandscapeSculptTool::Instance().setToolStrength(1.1f);

    //Landscape l(10, 10, 1);
    //l.refreshByLandscapeTool();

    //MainWindow w;
    //w.show();


    MainApplicationWindow w;
    w.resize(800, 800);
    w.show();
    //MainWindow w;
    //w.show();




    return a.exec();
}
