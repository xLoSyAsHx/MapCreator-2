#include "camera3d.h"

#include <QOpenGLShaderProgram>

Camera3D::Camera3D() :
    m_scale(1.0f),
    m_direction(QVector3D(0, 0, -0.1))
{
    m_globalTransform.setToIdentity();
}

void Camera3D::rotate(const QQuaternion &rotation)
{
    m_rotate = rotation * m_rotate;

    m_direction = m_rotate
            //.inverted()
            .rotatedVector(QVector3D(0, 0, -0.1));

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

    QMatrix4x4 view;
    //view.setToIdentity();
    view.lookAt(m_translate, m_translate + m_direction, QVector3D(0, 1, 0));


    shaderProgram->setUniformValue("u_viewMatrix", view);
}

Camera3D &Camera3D::Instance()
{
    static Camera3D camera3D;
    return camera3D;
}

QQuaternion Camera3D::getRotation() const
{
    return m_rotate;
}

QVector3D Camera3D::getPosition() const
{
    return m_translate;
}

QVector3D Camera3D::getDirection() const
{
    return m_direction;
}

void Camera3D::setDirection(QVector3D direction)
{
    m_direction = direction;
}

QMatrix4x4 Camera3D::getViewMatrix() const
{
    QMatrix4x4 view;
    //view.setToIdentity();
    view.lookAt(m_translate, m_translate + m_direction, QVector3D(0, 1, 0));

    return view;
}

void Camera3D::lookAt()
{
    //m_viewMatrix.lookAt(m_translate, m_translate + QVector3D(0, 0, -1), QVector3D(0, 1, 0));
}

void Camera3D::m_resetViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix =  m_viewMatrix * m_globalTransform.inverted();
}
