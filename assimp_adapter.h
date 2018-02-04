#ifndef ASSIMP_ADAPTER_H
#define ASSIMP_ADAPTER_H

#include <QVector3D>
#include <QMatrix4x4>

#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>

QVector3D toQVector3D(const aiVector3t<float> & vec3) {
    return QVector3D(vec3.x, vec3.y, vec3.z);
}

/*
QVector3D toQVector3D(const aiVector3D & vec3) {
    return QVector3D(vec3.x, vec3.y, vec3.z);
}
*/

QMatrix4x4 toQMatrix4x4(const aiMatrix4x4t<float> & mat)  {
    return QMatrix4x4(mat.a1, mat.a2, mat.a3, mat.a4,
                      mat.b1, mat.b2, mat.b3, mat.b4,
                      mat.c1, mat.c2, mat.c3, mat.c4,
                      mat.d1, mat.d2, mat.d3, mat.d4);
}

#endif // ASSIMP_ADAPTER_H
