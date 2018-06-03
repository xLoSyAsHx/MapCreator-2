#ifndef PICKINGTEXTURE_H
#define PICKINGTEXTURE_H


#include <QVector4D>

#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

class PickingTexture
{
public:

    PickingTexture();

    bool Init(uint windowWidth, uint windowHeight, QOpenGLFunctions *glFunctions);

    void EnableWriting(QOpenGLFunctions *glFunctions);

    void DisableWriting(QOpenGLFunctions *glFunctions);


    QVector4D ReadPixel(uint x, uint y, QOpenGLFunctions *glFunctions);

private:
    GLuint m_fbo;
    GLuint m_pickingTexture;
    GLuint m_depthTexture;

    int m_windowWidth;
    int m_windowHeight;
};

#endif // PICKINGTEXTURE_H
