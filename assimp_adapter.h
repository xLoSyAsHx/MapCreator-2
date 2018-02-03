#ifndef ASSIMP_ADAPTER_H
#define ASSIMP_ADAPTER_H

#include <QVector3D>

#include <assimp/scene.h>

void QVector3D(const aiVector3D & pVec3) const {
    return QVector(pVec3.x, pVec3.y, pVec3.z);
}


#endif // ASSIMP_ADAPTER_H
