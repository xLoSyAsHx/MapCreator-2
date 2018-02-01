#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/material.h>

#include <QString>
#include <QVector3D>
#include <QImage>
#include <QMultiMap>

#include <QSharedPointer>


struct Material
{
public:

    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;

    void addTexture(aiTextureType type, QImage image) {
        m_textures.insert(type, image);
    }

private:

    QMultiMap<aiTextureType, QImage> m_textures;
};

#endif // MATERIAL_H
