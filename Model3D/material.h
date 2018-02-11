#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/material.h>

#include <QString>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QMap>
#include <QOpenGLShaderProgram>

#include <QSharedPointer>


struct Material
{
public:


    QString Name;
    QVector3D Ambient;
    QVector3D Diffuse;
    QVector3D Specular;
    float Shininess;

    void addTexture(aiTextureType type, QOpenGLTexture *image) {
        m_textures[type] = image;
    }

    bool bind(aiTextureType type, uint textIndex) {
        auto it = m_textures.find(type);

        if (it != m_textures.end()) {
            (*it)->bind(textIndex);
            return true;
        }

        return false;
    }

    void release(aiTextureType type) {
        auto it = m_textures.find(type);

        if (it != m_textures.end())
            (*it)->release();
    }

    bool isBound(aiTextureType type) const {
        auto it = m_textures.find(type);

        if (it != m_textures.end()) {
            return (*it)->isBound();
        }

        return false;
    }

private:

    QMap<aiTextureType, QOpenGLTexture*> m_textures;
};

#endif // MATERIAL_H
