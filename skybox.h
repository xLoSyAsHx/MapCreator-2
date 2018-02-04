#ifndef SKYBOX_H
#define SKYBOX_H

#include <QImage>

#include "transformational.h"


class SkyBox : public Transformational
{
public:
    SkyBox(float width, const QImage &texture);
    ~SkyBox();

    // Transformational interface
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    // End Transformational interface

private:
    class SimpleObject3D* m_cube;
};

#endif // SKYBOX_H
