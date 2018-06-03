#ifndef LANDSCAPEWIDGET_H
#define LANDSCAPEWIDGET_H

#include <QDockWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QFileDialog>

#include <QInputDialog>

#include <QLabel>
#include <QLineEdit>

#include "landscape.h"
#include "system.h"

class LandscapeWidget : public QDockWidget
{
public slots:
    void onCreateLandscape();
    void onDeleteLandscape();
    void onAddTexture();
    void onSculptToolStatusChanged();
    //void addButton();
    //void addItemToScene(QListWidgetItem *item);

public:
    LandscapeWidget(System* system);
    ~LandscapeWidget();


    //void addObject(QString pathToFile);

private:
    System *m_system;
    QWidget *m_widget;
    QLineEdit *m_landscapeWidth;
    QLineEdit *m_landscapeHeight;
    QPushButton *m_createButton;

    bool isCreated = false;
};

#endif // LANDSCAPEWIDGET_H
