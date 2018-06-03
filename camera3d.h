#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <QVector>
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

#include "transformational.h"

class Camera3D : public Transformational
{
public:

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(class QOpenGLShaderProgram *program, class QOpenGLFunctions *functions = nullptr) override;
    void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override {}
    // End Transformational interface

    static Camera3D& Instance();

    QQuaternion getRotation() const;
    QVector3D getPosition() const;
    QVector3D getDirection() const;
    void setDirection(QVector3D direction);

    QMatrix4x4 getViewMatrix() const;

    void lookAt();

private:

    Camera3D();
    Camera3D(const Camera3D&) = delete;
    Camera3D& operator=(const Camera3D&) = delete;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    QVector3D m_direction;
    QMatrix4x4 m_viewMatrix;



    void m_resetViewMatrix();
};

#endif // CAMERA3D_H
