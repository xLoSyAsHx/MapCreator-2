#ifndef MODEL3D_2_H
#define MODEL3D_2_H

// C++11 includes
#include <memory>

// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>


#include <QOpenGLShaderProgram>

#include "mesh.h"
#include "node.h"
#include "material.h"

class Model3D_2
{
public:
    Model3D_2(QString path);

    void draw(QOpenGLShaderProgram *shaderProgram);

private:
    QSharedPointer<Node> m_rootNode;
    QVector<std::unique_ptr<Mesh>> m_meshes;
    QVector<std::unique_ptr<Material>> m_materials;

    QMatrix4x4 m_globalTransformation;

};

#endif // MODEL3D_2_H
