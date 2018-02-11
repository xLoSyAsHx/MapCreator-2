#ifndef TRANSFORMATIONAL_H
#define TRANSFORMATIONAL_H

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

class Transformational {
public:

    virtual void rotate(const QQuaternion &rotation) = 0;
    virtual void translate(const QVector3D &translation) = 0;
    virtual void scale(const float scaleKoef) = 0;
    virtual void setGlobalTransform(const QMatrix4x4 &matrix) = 0;
    virtual void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) = 0;

    virtual ~Transformational() {}
};

#endif // TRANSFORMATIONAL_H
