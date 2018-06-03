#include "mainapplicationwindow.h"

#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>

MainApplicationWindow::MainApplicationWindow()
{
    QLayout *l = new QHBoxLayout;

    m_system = new System(this);
    setCentralWidget(m_system);

    m_objects3DList = new Objects3DList();
    m_objects3DList->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_objects3DList);

    m_landscapeWidget = new LandscapeWidget(m_system);
    m_landscapeWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_landscapeWidget);

    m_objectParametersWidget = new ObjectParametersWidget(m_system);
    m_objectParametersWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_objectParametersWidget);

    ObjectManager::Instance().setObjectParamsWidget(m_objectParametersWidget);


    QMenuBar *menuBar = new QMenuBar(this);

    // Menu Windows
    QMenu* menuWindows = new QMenu("&Windows", this);

    menuWindows->addAction("Objects window",
                           this,
                           MainApplicationWindow::showObjectsWindow_DockWidget,
                           Qt::CTRL + Qt::Key_Q
                          );
    menuWindows->addAction("Landscape window",
                           this,
                           MainApplicationWindow::showLandscapeCreation_DockWidget,
                           Qt::CTRL + Qt::Key_Q
                          );
    menuWindows->addAction("Object parameters",
                           this,
                           MainApplicationWindow::showObjectParameters_DockWidget,
                           Qt::CTRL + Qt::Key_Q
                          );

    // Menu Help
    QMenu* menuHelp = new QMenu("&Help", this);
    menuHelp->addAction("&About Qt",
                           this,
                           SLOT(aboutQt()),
                           Qt::CTRL + Qt::Key_Q
                          );

    menuBar->addMenu(menuWindows);
    menuBar->addMenu(menuHelp);

    setMenuBar(menuBar);
}

void MainApplicationWindow::showObjectsWindow_DockWidget()
{
    m_objects3DList->setVisible(true);
}

void MainApplicationWindow::showLandscapeCreation_DockWidget()
{
    m_landscapeWidget->setVisible(true);
}

void MainApplicationWindow::showObjectParameters_DockWidget()
{
    m_objectParametersWidget->setVisible(true);
}
