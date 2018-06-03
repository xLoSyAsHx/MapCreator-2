#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QVector>
#include <QMap>
#include <QVector4D>
#include <QSharedPointer>

#include "transformational.h"

#include "Model3D_4/model3d.h"

#include "SystemWidgets/objectparameterswidget.h"

class ObjectManager
{
public:
    static ObjectManager& Instance();

    void setObjectParamsWidget(
            ObjectParametersWidget *objectParamsWidget);

    void addObject(QString pathToFile);

    void changeSelectionStatus(QVector4D objId);
    void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions);
    void drawSelectionCubes(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions);
    void objectPicking(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);

    void translateSelectedObjects(QVector3D shift);
    void translateSelectedObjects(QQuaternion q);


    int getNumOfSelectedObjects() const;

private:

    ObjectManager(){}
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;


    int m_lastId_a = 1;
    int m_lastId_b = 1;
    int m_lastId_c = 1;
    int m_lastId_d = 0;

    QVector<QSharedPointer<Transformational>> m_objects;
    QMap<int, QSharedPointer<Transformational>> m_selectedObjects;

    ObjectParametersWidget *m_objectParamsWidget;


    QVector4D GenerateObjectId();
    int translateVec4IdToPosition(QVector4D vec);




};

#endif // OBJECTMANAGER_H
