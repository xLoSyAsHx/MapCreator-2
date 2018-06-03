#include "objectmanager.h"



ObjectManager &ObjectManager::Instance()
{
    static ObjectManager ObjectManager;
    return ObjectManager;
}

void ObjectManager::setObjectParamsWidget(
        ObjectParametersWidget *objectParamsWidget)
{
    m_objectParamsWidget = objectParamsWidget;
}

void ObjectManager::addObject(QString pathToFile)
{
    Model3D *model = new Model3D(GenerateObjectId());
    model->loadFromFile(pathToFile);

    m_objects.append(QSharedPointer<Transformational>(model));
}

void ObjectManager::changeSelectionStatus(QVector4D objId)
{
    if (objId.isNull())
        return;

    int objectPosition = translateVec4IdToPosition(objId);

    qDebug() << objectPosition;

    QSharedPointer<Transformational> tr = m_objects[objectPosition];

    if (tr->isSelected())
    {
        m_selectedObjects.remove(m_selectedObjects.find(objectPosition).key());
        tr->setObjectSelection(false);
    }
    else
    {
        m_selectedObjects[objectPosition] = tr;
        tr->setObjectSelection(true);
    }

    if (m_selectedObjects.size() == 1)
    {
        m_objectParamsWidget->handleOneObjectNewPosition(tr->getPosition());
    }
}

void ObjectManager::draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions)
{
    foreach (QSharedPointer<Transformational> obj, m_objects) {
        obj->draw(programm, functions);
    }
}

void ObjectManager::drawSelectionCubes(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    foreach (QSharedPointer<Transformational> obj, m_objects) {
        obj->drawSelectionCubes(program, functions);
    }
}

void ObjectManager::objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions)
{
    foreach (QSharedPointer<Transformational> obj, m_objects) {
        obj->objectPicking(programm, functions);
    }
}

void ObjectManager::translateSelectedObjects(QVector3D shift)
{
    for (QSharedPointer<Transformational> obj : m_selectedObjects)
        obj->translate(shift);
}

void ObjectManager::translateSelectedObjects(QQuaternion q)
{
    for (QSharedPointer<Transformational> obj : m_selectedObjects)
        obj->rotate(q);
}

int ObjectManager::getNumOfSelectedObjects() const
{
    return m_selectedObjects.size();
}

QVector4D ObjectManager::GenerateObjectId()
{
    ++m_lastId_d;

    if (m_lastId_d > 255)
    {
        ++m_lastId_c;
        m_lastId_d = 1;
    }
    \

    if (m_lastId_c > 255)
    {
        ++m_lastId_b;
        m_lastId_c = 1;
    }

    if (m_lastId_b > 255)
    {
        ++m_lastId_a;
        m_lastId_b = 1;
    }

    return QVector4D(
                float(m_lastId_a) / 255.0f,
                float(m_lastId_b) / 255.0f,
                float(m_lastId_c) / 255.0f,
                float(m_lastId_d) / 255.0f);
}

int ObjectManager::translateVec4IdToPosition(QVector4D vec)
{
    static int toInt = 255;
    static int cMultiplier = 255;
    static int bMultiplier = 255 * 255;
    static int aMultiplier = 255 * 255 * 255;

    int pos = 0;

    pos += (vec.w() * toInt - 1);
    pos += (vec.z() * toInt - 1) * cMultiplier;
    pos += (vec.y() * toInt - 1) * bMultiplier;
    pos += (vec.x() * toInt - 1) * aMultiplier;

    return pos;
}

