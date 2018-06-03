#ifndef OBJECTPARAMETERSWIDGET_H
#define OBJECTPARAMETERSWIDGET_H


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

#include <QVector3D>

class System;

class ObjectParametersWidget : public QDockWidget
{
public slots:
    void handleNewObjectPosions(QVector3D shift);
    void handleOneObjectNewPosition(QVector3D newPos);
    void handleNewObjectRotations(QQuaternion q);

    void translateObjects_X_UP();
    void translateObjects_X_DOWN();

    void translateObjects_Y_UP();
    void translateObjects_Y_DOWN();

    void translateObjects_Z_UP();
    void translateObjects_Z_DOWN();


    void rotateObjects_X_UP();
    void rotateObjects_X_DOWN();

    void rotateObjects_Y_UP();
    void rotateObjects_Y_DOWN();

    void rotateObjects_Z_UP();
    void rotateObjects_Z_DOWN();

public:
    ObjectParametersWidget(System* system);



private:
    System* m_system;

    struct Labels_3 {
        QLineEdit *xVal, *yVal, *zVal;

        QVector3D getVector3D()
        {
            return QVector3D(
                      xVal->text().toFloat(),
                      yVal->text().toFloat(),
                      zVal->text().toFloat()
                  );
        }

        void update(QVector3D newPos)
        {
            xVal->setText(QString::number(newPos.x()));
            yVal->setText(QString::number(newPos.y()));
            zVal->setText(QString::number(newPos.z()));
        }
    };

    QWidget *m_widget;

    Labels_3 m_Labels_3_pos;
    Labels_3 m_Labels_3_rot;

    QGridLayout* createPositionGrid(int fontSize, int arrowSize);
    QGridLayout* createRotationGrid(int fontSize, int arrowSize);
    QGridLayout* createScaleGrid(int fontSize, int arrowSize);
};

#endif // OBJECTPARAMETERSWIDGET_H
