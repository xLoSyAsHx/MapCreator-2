#include "camera3d.h"

#include <QOpenGLShaderProgram>

Camera3D::Camera3D() :
    m_scale(1.0f)
{
    m_globalTransform.setToIdentity();
}

void Camera3D::rotate(const QQuaternion &rotation)
{
    m_rotate = rotation * m_rotate;
    m_resetViewMatrix();
}

void Camera3D::translate(const QVector3D &translation)
{
    m_translate += translation;
    m_resetViewMatrix();
}

void Camera3D::scale(const float scaleKoef)
{
    m_scale *= scaleKoef;
    m_resetViewMatrix();
}

void Camera3D::setGlobalTransform(const QMatrix4x4 &transformMatrix)
{
    m_globalTransform = transformMatrix;
    m_resetViewMatrix();
}

void Camera3D::draw(class QOpenGLShaderProgram *shaderProgram, class QOpenGLFunctions *functions)
{
    if (functions != nullptr)
        return;

    shaderProgram->setUniformValue("u_viewMatrix", m_viewMatrix);
}

void Camera3D::m_resetViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix =  m_viewMatrix * m_globalTransform.inverted();
}
