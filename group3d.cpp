#include "group3d.h"

Group3D::Group3D() :
    m_scale(1.0f)
{

}

void Group3D::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for (auto it = m_objects.cbegin(); it != m_objects.cend(); ++it) {
        (*it)->setGlobalTransform(localMatrix);
        (*it)->draw(shaderProgram, functions);
    }

}

void Group3D::addObject(Transformational *obj)
{
    m_objects.append(obj);
}

void Group3D::rotate(const QQuaternion &rotation)
{
    m_rotate = rotation * m_rotate;
}

void Group3D::translate(const QVector3D &translation)
{
    m_translate += translation;
}

void Group3D::scale(const float scaleKoef)
{
    m_scale *= scaleKoef;
}

void Group3D::setGlobalTransform(const QMatrix4x4 &transformMatrix)
{
    m_globalTransform = transformMatrix;
}
