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
    Camera3D();

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(class QOpenGLShaderProgram *program, class QOpenGLFunctions *functions = nullptr) override;
    // End Transformational interface


private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    QMatrix4x4 m_viewMatrix;



    void m_resetViewMatrix();
};

#endif // CAMERA3D_H
