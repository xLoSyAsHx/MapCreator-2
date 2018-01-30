#ifndef MATERIAL_H
#define MATERIAL_H


#include <QString>
#include <QVector3D>

struct Material
{
public:

    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;
};

#endif // MATERIAL_H
