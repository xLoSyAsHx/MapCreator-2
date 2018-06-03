#include "objectparameterswidget.h"



#include <QQuaternion>

#include <QPushButton>
#include "system.h"


void ObjectParametersWidget::handleNewObjectPosions(QVector3D shift)
{
    QVector3D newPos = m_Labels_3_pos.getVector3D() + shift;
    m_Labels_3_pos.update(newPos);
}

void ObjectParametersWidget::handleOneObjectNewPosition(QVector3D newPos)
{
    m_Labels_3_pos.update(newPos);
}

void ObjectParametersWidget::handleNewObjectRotations(QQuaternion q)
{
    QVector3D vec;
    float angle;
    q.getAxisAndAngle(&vec, &angle);
    m_Labels_3_rot.update(m_Labels_3_rot.getVector3D() + vec * qRound(angle));
}

void ObjectParametersWidget::translateObjects_X_UP()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(1, 0, 0));
    handleNewObjectPosions(QVector3D(1, 0, 0));
    m_system->update();
}

void ObjectParametersWidget::translateObjects_X_DOWN()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(-1, 0, 0));
    handleNewObjectPosions(QVector3D(-1, 0, 0));
    m_system->update();
}

void ObjectParametersWidget::translateObjects_Y_UP()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(0, 1, 0));
    handleNewObjectPosions(QVector3D(0, 1, 0));
    m_system->update();
}

void ObjectParametersWidget::translateObjects_Y_DOWN()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(0, -1, 0));
    handleNewObjectPosions(QVector3D(0, -1, 0));
    m_system->update();
}

void ObjectParametersWidget::translateObjects_Z_UP()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(0, 0, 1));
    handleNewObjectPosions(QVector3D(0, 0, 1));
    m_system->update();
}

void ObjectParametersWidget::translateObjects_Z_DOWN()
{
    ObjectManager::Instance().translateSelectedObjects(QVector3D(0, 0, -1));
    handleNewObjectPosions(QVector3D(0, 0, -1));
    m_system->update();
}

void ObjectParametersWidget::rotateObjects_X_UP()
{
    QVector3D dir = QVector3D(1, 0, 0);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();

}

void ObjectParametersWidget::rotateObjects_X_DOWN()
{
    QVector3D dir = QVector3D(-1, 0, 0);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();
}

void ObjectParametersWidget::rotateObjects_Y_UP()
{
    QVector3D dir = QVector3D(0, 1, 0);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();
}

void ObjectParametersWidget::rotateObjects_Y_DOWN()
{
    QVector3D dir = QVector3D(0, -1, 0);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();
}

void ObjectParametersWidget::rotateObjects_Z_UP()
{
    QVector3D dir = QVector3D(0, 0, 1);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();
}

void ObjectParametersWidget::rotateObjects_Z_DOWN()
{
    QVector3D dir = QVector3D(0, 0, -1);
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(dir, 10.0f);
    ObjectManager::Instance().translateSelectedObjects(rotation);
    handleNewObjectRotations(rotation);
    m_system->update();
}

ObjectParametersWidget::ObjectParametersWidget(System* system) :
    m_system(system)
{
    connect(system, System::onObjectsPositionChanged, this, handleNewObjectPosions);

    int fontSize = 13;
    int arrowSize = 8;

    m_widget = new QWidget(this);
    QVBoxLayout *mainLayout_v = new QVBoxLayout;

    // Horizontal layout 1
    QHBoxLayout *layout1_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *landscapeWidgetName = new QLabel("Object parameters");
    landscapeWidgetName->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout1_h->addWidget(landscapeWidgetName);


    // Horizontal layout 2
    QHBoxLayout *layout2_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *emptyLabel = new QLabel("");
    emptyLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout2_h->addWidget(emptyLabel);


    // Horizontal layout 3
    QHBoxLayout *layout3_h = new QHBoxLayout;
    layout3_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    emptyLabel = new QLabel("");
    emptyLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout3_h->addWidget(emptyLabel);


    QGridLayout *positionLayout = createPositionGrid(fontSize, arrowSize);
    QGridLayout *rotationLayout = createRotationGrid(fontSize, arrowSize);


    // Set up main layout
    mainLayout_v->addLayout(layout1_h);
    mainLayout_v->addLayout(layout2_h);
    mainLayout_v->addLayout(positionLayout);
    mainLayout_v->addLayout(layout3_h);
    mainLayout_v->addLayout(rotationLayout);
    mainLayout_v->setAlignment(Qt::AlignTop);

    m_widget->setLayout(mainLayout_v);
    setWidget(m_widget);
    setWindowTitle("Object parameters");

}

QGridLayout *ObjectParametersWidget::createPositionGrid(int fontSize, int arrowSize)
{
    // Grid layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout ->setAlignment(Qt::AlignTop);

    // Grid row 1
    QLabel *xPos = new QLabel("X");
    xPos->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    xPos->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(xPos, 0, 1);

    QLabel *yPos = new QLabel("Y");
    yPos->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    yPos->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(yPos, 0, 2);

    QLabel *zPos = new QLabel("Z");
    zPos->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    zPos->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(zPos, 0, 3);


    // Grid row 2
    QPushButton *xUpPosButton = new QPushButton("up");
    xUpPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(xUpPosButton, 1, 1);
    connect(xUpPosButton, QPushButton::clicked, this, translateObjects_X_UP);

    QPushButton *yUpPosButton = new QPushButton("up");
    yUpPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(yUpPosButton, 1, 2);
    connect(yUpPosButton, QPushButton::clicked, this, translateObjects_Y_UP);

    QPushButton *zUpPosButton = new QPushButton("up");
    zUpPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(zUpPosButton, 1, 3);
    connect(zUpPosButton, QPushButton::clicked, this, translateObjects_Z_UP);


    // Grid row 3
    QLabel *positionLabel = new QLabel("Position");
    positionLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    positionLabel->setAlignment(Qt::AlignLeft);
    gridLayout->addWidget(positionLabel, 2, 0);

    m_Labels_3_pos.xVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_pos.xVal, 2, 1);

    m_Labels_3_pos.yVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_pos.yVal, 2, 2);

    m_Labels_3_pos.zVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_pos.zVal, 2, 3);

    // Grid row 4
    QPushButton *xDownPosButton = new QPushButton("down");
    xDownPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(xDownPosButton, 3, 1);
    connect(xDownPosButton, QPushButton::clicked, this, translateObjects_X_DOWN);

    QPushButton *yDownPosButton = new QPushButton("down");
    yDownPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(yDownPosButton, 3, 2);
    connect(yDownPosButton, QPushButton::clicked, this, translateObjects_Y_DOWN);

    QPushButton *zDownPosButton = new QPushButton("down");
    zDownPosButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(zDownPosButton, 3, 3);
    connect(zDownPosButton, QPushButton::clicked, this, translateObjects_Z_DOWN);

    return gridLayout;
}

QGridLayout *ObjectParametersWidget::createRotationGrid(int fontSize, int arrowSize)
{
    // Grid layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout ->setAlignment(Qt::AlignTop);

    // Grid row 1
    QLabel *xRot = new QLabel("X");
    xRot->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    xRot->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(xRot, 0, 1);

    QLabel *yRot = new QLabel("Y");
    yRot->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    yRot->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(yRot, 0, 2);

    QLabel *zRot = new QLabel("Z");
    zRot->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    zRot->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(zRot, 0, 3);


    // Grid row 2
    QPushButton *xUpRotButton = new QPushButton("up");
    xUpRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(xUpRotButton, 1, 1);
    connect(xUpRotButton, QPushButton::clicked, this, rotateObjects_X_UP);

    QPushButton *yUpRotButton = new QPushButton("up");
    yUpRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(yUpRotButton, 1, 2);
    connect(yUpRotButton, QPushButton::clicked, this, rotateObjects_Y_UP);

    QPushButton *zUpRotButton = new QPushButton("up");
    zUpRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(zUpRotButton, 1, 3);
    connect(zUpRotButton, QPushButton::clicked, this, rotateObjects_Z_UP);


    // Grid row 3
    QLabel *rotationLabel = new QLabel("Rotation");
    rotationLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    rotationLabel->setAlignment(Qt::AlignLeft);
    gridLayout->addWidget(rotationLabel, 2, 0);

    m_Labels_3_rot.xVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_rot.xVal, 2, 1);

    m_Labels_3_rot.yVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_rot.yVal, 2, 2);

    m_Labels_3_rot.zVal = new QLineEdit;
    gridLayout->addWidget(m_Labels_3_rot.zVal, 2, 3);

    // Grid row 4
    QPushButton *xDownRotButton = new QPushButton("down");
    xDownRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(xDownRotButton, 3, 1);
    connect(xDownRotButton, QPushButton::clicked, this, rotateObjects_X_DOWN);

    QPushButton *yDownRotButton = new QPushButton("down");
    yDownRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(yDownRotButton, 3, 2);
    connect(yDownRotButton, QPushButton::clicked, this, rotateObjects_Y_DOWN);

    QPushButton *zDownRotButton = new QPushButton("down");
    zDownRotButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(zDownRotButton, 3, 3);
    connect(zDownRotButton, QPushButton::clicked, this, rotateObjects_Z_DOWN);

    return gridLayout;
}

QGridLayout *ObjectParametersWidget::createScaleGrid(int fontSize, int arrowSize)
{

}
