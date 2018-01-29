#ifndef MODEL3D_2_H
#define MODEL3D_2_H

#include <QString>
#include <QVector>
#include <QMatrix4x4>

#include <QOpenGLShaderProgram>

class Model3D_2
{
public:
    Model3D_2(QString path);

    void draw(QOpenGLShaderProgram *shaderProgram);

private:
    QVector<Mesh*> m_meshes;
    QMatrix4x4 m_globalPosition;

};

#endif // MODEL3D_2_H
