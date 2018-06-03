#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H


#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>

#include "transformational.h"
#include "Model3D/vertexdata.h"


class SimpleObject3D : public Transformational
{
public:
    SimpleObject3D();
    SimpleObject3D(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);

    ~SimpleObject3D();

    void init(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    void draw(class QOpenGLShaderProgram *programm, class QOpenGLFunctions *functions);
    void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override {}

    void rotate(const class QQuaternion &rotation);
    void translate(const class QVector3D &translation);
    void scale(const float scaleKoef);
    void setGlobalTransform(const class QMatrix4x4 &transformMatrix);

    QVector3D getPosition() const override;


private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

    class QOpenGLTexture *m_texture;


    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
};

#endif // SIMPLEOBJECT3D_H
