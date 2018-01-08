#include "simpleobject3d.h"

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

SimpleObject3D::SimpleObject3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr),
    m_scale(1.0f)
{

}

SimpleObject3D::SimpleObject3D(QVector<VertexData> &vertData,
                               const QVector<GLuint> &indexes, const QImage &texture) :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr),
    m_scale(1.0f)
{
    init(vertData, indexes, texture);
}

SimpleObject3D::~SimpleObject3D()
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture!= nullptr && m_texture->isCreated())
        m_texture->destroy();

}

void SimpleObject3D::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);
    modelMatrix = m_globalTransform * modelMatrix;

    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated())
        return;

    // Bind texture
    m_texture->bind(0);
    shaderProgram->setUniformValue("u_texture", 0);

    // Set uniform values
    shaderProgram->setUniformValue("u_modelMatrix", modelMatrix);

    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    int offset = 0;

    // Set varying attribute "a_position"
    int location = shaderProgram->attributeLocation("a_position");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));


    offset += sizeof(QVector3D);
    // Set varying attribute "a_textCoord"
    location = shaderProgram->attributeLocation("a_textCoord");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));


    offset += sizeof(QVector2D);
    // Set varying attribute "a_normal"
    location = shaderProgram->attributeLocation("a_normal");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw
    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    // Release
    m_indexBuffer.release();
    m_vertexBuffer.release();
    m_texture->release();
}

void SimpleObject3D::rotate(const QQuaternion &rotation)
{
    m_rotate = rotation * m_rotate;
}

void SimpleObject3D::translate(const QVector3D &translation)
{
    m_translate += translation;
}

void SimpleObject3D::scale(const float scaleKoef)
{
    m_scale *= scaleKoef;
}

void SimpleObject3D::setGlobalTransform(const QMatrix4x4 &transformMatrix)
{
    m_globalTransform = transformMatrix;
}

void SimpleObject3D::init(QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture)
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();

    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if (m_texture!= nullptr && m_texture->isCreated()) {
        delete m_texture;
        m_texture = nullptr;
    }

    // Create and alloc memory for vertex buffer
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    // Create and alloc memory for index buffer
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}
