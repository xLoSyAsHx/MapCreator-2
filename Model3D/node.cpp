#include "node.h"


#include <QMap>
#include <memory>
#include "mesh.h"


void Node::draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4 transformation)
{
    // For model loader (configurate bones)
    QMap<QString, bool> map;

    map[m_name] = false;

    QVector<std::unique_ptr<Mesh>> m_meshes;
    for (auto it = m_meshIndexes.begin(); it != m_meshes.end(); ++it) {
        m_meshes[*it].get()->fillMap(map);
    }


    for (auto it = map.begin(); it != map.end(); ++it) {

    }
}