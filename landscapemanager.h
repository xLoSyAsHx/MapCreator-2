#ifndef LANDSCAPEMANAGER_H
#define LANDSCAPEMANAGER_H


#include <QVector>
#include <QMap>

#include <QVector4D>
#include <QSharedPointer>
#include <QOpenGLTexture>

#include "plane3d.h"

#include "landscape.h"

class LandscapeManager
{
public:
    static LandscapeManager& Instance();

    void createLandscape(uint width, uint heightint, uint blockSize);
    void destroy();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool isSculptToolUse);
    void objectPicking(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void updateLandscapeToolPosition(
            QVector3D mouseDir, QVector4D triangleId, QMatrix4x4 projectionMatrix);

    void addTexture(aiTextureType type, QOpenGLTexture *image);

    void refreshByLandscapeTool();

public slots:
    void updateLandscape();

private:

    LandscapeManager();
    LandscapeManager(const LandscapeManager&) = delete;
    LandscapeManager& operator=(const LandscapeManager&) = delete;

    int m_lastLandscapeId_a = 1;
    int m_lastLandscapeId_b = 1;
    int m_lastLandscapeId_c = 1;
    int m_lastLandscapeId_d = 1;

    QSharedPointer<Landscape> m_landscape;

    QVector4D GenerateObjectIdForLandscape();
    int translateVec4IdToPosition(QVector4D vec);

};

#endif // LANDSCAPEMANAGER_H
