#ifndef TRANSFORMATIONAL_H
#define TRANSFORMATIONAL_H


class Transformational {
public:

    virtual void rotate(const class QQuaternion &rotation) = 0;
    virtual void translate(const class QVector3D &translation) = 0;
    virtual void scale(const float scaleKoef) = 0;
    virtual void setGlobalTransform(const class QMatrix4x4 &matrix) = 0;
    virtual void draw(class QOpenGLShaderProgram *programm, class QOpenGLFunctions *functions) = 0;

};

#endif // TRANSFORMATIONAL_H
