#include "pickingtexture.h"


PickingTexture::PickingTexture()
{

}


bool PickingTexture::Init(uint windowWidth, uint windowHeight, QOpenGLFunctions *glFunctions)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    // Создание FBO
    glFunctions->glGenFramebuffers(1, &m_fbo);
    glFunctions->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    // Создание объекта текстуры для буфера с информацией о примитиве
    glGenTextures(1, &m_pickingTexture);
    glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glFunctions->glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);


    // Проверка FBO на корректность
    GLenum Status = glFunctions->glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // Возвращаем стандартный буфер кадра
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glFunctions->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void PickingTexture::EnableWriting(QOpenGLFunctions *glFunctions)
{
    glFunctions->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void PickingTexture::DisableWriting(QOpenGLFunctions *glFunctions)
{
    glFunctions->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

QVector4D PickingTexture::ReadPixel(uint x, uint y, QOpenGLFunctions *glFunctions)
{
    glFunctions->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

    glReadBuffer(GL_COLOR_ATTACHMENT0);

    QVector4D Pixel;
    glReadPixels(x, m_windowHeight - y, 1, 1, GL_RGBA, GL_FLOAT, &Pixel);

    glReadBuffer(GL_NONE);

    glFunctions->glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return Pixel;
}
