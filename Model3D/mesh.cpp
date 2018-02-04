#include "Model3D/mesh.h"

#include <QMap>

Mesh::Mesh()
{

}

void Mesh::fillMap(QMap<QString, bool> &map)
{
    for (auto it = m_bones.begin(); it != m_bones.end(); ++it) {

        if (map.find(it->data()->getName()) != map.end()) {
            map[it->data()->getName()] = true;
        }
    }
}

