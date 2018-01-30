#ifndef MESH_H
#define MESH_H


#include <QVector>
#include <QVector3D>
#include <QSharedPointer>

#include "bone.h"

class Mesh
{
public:
    Mesh();

    void fillMap(QMap<QString, bool> &map);

private:
    QVector<QVector3D> m_bitangents;
    QVector<QSharedPointer<Bone>> m_bones;
};

#endif // MESH_H
