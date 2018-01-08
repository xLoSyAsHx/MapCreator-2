#ifndef SKYBOX_H
#define SKYBOX_H

#include "transformational.h"

class SimpleObject3D;
class QImage;

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
    SimpleObject3D* m_cube;
};

#endif // SKYBOX_H
