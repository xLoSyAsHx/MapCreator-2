#include "Model3D/vertexdata.h"

VertexData::VertexData()
{
}


VertexData::VertexData(QVector3D p, QVector2D t, QVector3D n) :
    position(p), texCoord(t), normal(n)
{
}
