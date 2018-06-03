#ifndef TRANSFORMATIONAL_H
#define TRANSFORMATIONAL_H

#include <QVector4D>

class QQuaternion;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

class Transformational {

    bool m_isSelected = false;
public:

    virtual void rotate(const QQuaternion &rotation) = 0;
    virtual void translate(const QVector3D &translation) = 0;
    virtual void scale(const float scaleKoef) = 0;
    virtual void setGlobalTransform(const QMatrix4x4 &matrix) = 0;
    virtual void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) = 0;
    virtual void drawSelectionCubes(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) {}
    virtual void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) = 0;

    void setObjectSelection(bool selectedStatus) { m_isSelected = selectedStatus; }
    bool isSelected() { return m_isSelected; }

    virtual QVector3D getPosition() const = 0;

    virtual ~Transformational() {}
};

#endif // TRANSFORMATIONAL_H
