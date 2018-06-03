#ifndef GROUP3D_H
#define GROUP3D_H

#include <QVector>
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

#include "transformational.h"

class Group3D: public Transformational
{
public:
    Group3D();

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions) override;
    void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override {}

    QVector3D getPosition() const override;
    // End Transformational interface

    void addObject(Transformational *obj);


private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    QVector<Transformational*> m_objects;



    void m_formAndSetGlobalTransformForObjects(int pos = -1);

};

#endif // GROUP3D_H
