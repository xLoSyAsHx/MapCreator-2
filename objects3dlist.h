#ifndef OBJECTS3DLIST_H
#define OBJECTS3DLIST_H

#include <QWidget>

#include <QDockWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QPushButton>

#include <QFileDialog>

#include <QInputDialog>

#include <QLabel>

#include "widgetforlist.h"
#include "objectmanager.h"

class Objects3DList : public QDockWidget
{
public slots:
    void addButton();
    void addItemToScene(QListWidgetItem *item);

public:
    Objects3DList();
    ~Objects3DList();


    void addObject(QString pathToFile);

private:

    QListWidget *m_listWidget;
};

#endif // OBJECTS3DLIST_H
