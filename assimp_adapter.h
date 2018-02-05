#ifndef ASSIMP_ADAPTER_H
#define ASSIMP_ADAPTER_H

#include <QVector3D>
#include <QMatrix4x4>

#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

QVector3D toQVector3D(const aiVector3t<float> & vec3);

/*
QVector3D toQVector3D(const aiVector3D & vec3) {
    return QVector3D(vec3.x, vec3.y, vec3.z);
}
*/

QMatrix4x4 toQMatrix4x4(const aiMatrix4x4t<float> & mat);

#endif // ASSIMP_ADAPTER_H
