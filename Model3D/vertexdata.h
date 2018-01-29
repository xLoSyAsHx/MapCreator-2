#ifndef VERTEXDATA_H
#define VERTEXDATA_H


#include <QVector3D>

struct VertexData {
    VertexData();
    VertexData(QVector3D p, QVector2D t, QVector3D n);

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

#endif // VERTEXDATA_H
