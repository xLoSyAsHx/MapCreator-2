#ifndef WIDGETFORLIST_H
#define WIDGETFORLIST_H

#include <QWidget>

class WidgetForList : public QWidget
{
public:
    WidgetForList();

    void setParent(QWidget *parent);

    QString pathToFile;
};

#endif // WIDGETFORLIST_H
