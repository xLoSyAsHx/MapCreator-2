#ifndef MAINAPPLICATIONWINDOW_H
#define MAINAPPLICATIONWINDOW_H


#include <QMainWindow>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "system.h"
#include "objects3dlist.h"
#include "landscapewidget.h"
#include "SystemWidgets/objectparameterswidget.h"

class MainApplicationWindow : public QMainWindow
{
public:
    MainApplicationWindow();


private:
    System *m_system;
    Objects3DList *m_objects3DList;
    LandscapeWidget *m_landscapeWidget;
    ObjectParametersWidget *m_objectParametersWidget;

private slots:
    void showObjectsWindow_DockWidget();
    void showLandscapeCreation_DockWidget();
    void showObjectParameters_DockWidget();
};

#endif // MAINAPPLICATIONWINDOW_H
