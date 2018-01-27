#include "Model3D.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Model3D::Model3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_normalBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_scale(1.0f)
{
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, (float)800/600, .3f, 1000);

    m_view.setToIdentity();
           m_view.lookAt(QVector3D(0.0f, 0.0f, 1.2f),    // Camera Position
                         QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
                         QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

    init();
}


Model3D::~Model3D()
{
}

void Model3D::drawNode(const QMatrix4x4 &model, const Node *node, QMatrix4x4 parent)
{
    // Prepare matrices
    QMatrix4x4 local = parent * node->transformation;
    QMatrix4x4 mv = m_view * model * local;

    m_pgm.setUniformValue("MV",  mv);
    m_pgm.setUniformValue("N",   mv.normalMatrix());
    m_pgm.setUniformValue("MVP", m_projection * mv);

    // Draw each mesh in this node
    for(int i = 0; node->meshes.size(); ++i)
    {
        const Mesh& m = *node->meshes[i];

        if (m.material->Name == QString("DefaultMaterial")) {
            m_pgm.setUniformValue("material.Ka",        QVector3D(  0.05f, 0.2f, 0.05f ));
            m_pgm.setUniformValue("material.Kd",        QVector3D(  0.3f,  0.5f, 0.3f  ));
            m_pgm.setUniformValue("material.Ks",        QVector3D(  0.6f,  0.6f, 0.6f  ));
            m_pgm.setUniformValue("material.shininess", 50.f);
        } else {
            m_pgm.setUniformValue("material.Ka",        m.material->Ambient);
            m_pgm.setUniformValue("material.Kd",        m.material->Diffuse);
            m_pgm.setUniformValue("material.Ks",        m.material->Specular);
            m_pgm.setUniformValue("material.shininess", m.material->Shininess);
        }

        glDrawElements(GL_TRIANGLES, m.indexCount, GL_UNSIGNED_INT, (const GLvoid*)(m.indexOffset * sizeof(GLuint)));
    }

    // Recursively draw this nodes children nodes
    for(int i = 0; i < node->nodes.size(); ++i)
        drawNode(model, &node->nodes[i], local);
}



void Model3D::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{
    m_pgm.bind();

    m_indexBuffer.bind();
    m_vertexBuffer.bind();
    m_normalBuffer.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(m_translate);
    modelMatrix.rotate(m_rotate);
    modelMatrix.scale(m_scale);
    modelMatrix = m_globalTransform * modelMatrix;

    QMatrix4x4 asd;
    drawNode(modelMatrix, m_rootNode.data(), asd);

    m_indexBuffer.release();
    m_vertexBuffer.release();
    m_normalBuffer.release();

    m_pgm.release();
}

void Model3D::init()
{
    if ( !m_pgm.addShaderFromSourceCode( QOpenGLShader::Vertex, vertexShader)) {
       qDebug() << "Error in vertex shader:" << m_pgm.log();
       exit(1);
    }
    if ( !m_pgm.addShaderFromSourceCode( QOpenGLShader::Fragment, fragmentShader)) {
       qDebug() << "Error in fragment shader:" << m_pgm.log();
       exit(1);
    }
    if ( !m_pgm.link() ) {
       qDebug() << "Error linking shader program:" << m_pgm.log();
       exit(1);
    }

    m_pgm.bind();

    m_pgm.setUniformValue("light.position",   QVector4D( -1.0f,  1.0f, 1.0f, 1.0f ));
    m_pgm.setUniformValue("light.intensity",  QVector3D(  1.0f,  1.0f, 1.0f  ));



    // Put all the vertex data in a FBO
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));
    m_vertexBuffer.release();

    // Configure the attribute stream
    m_pgm.enableAttributeArray(0);
    m_pgm.setAttributeBuffer(0, GL_FLOAT, 0, 3);

    // Put all the normal data in a FBO
    m_normalBuffer.create();
    m_normalBuffer.bind();
    m_normalBuffer.allocate(m_normals.constData(), m_normals.size() * sizeof(float));
    m_normalBuffer.release();

    // Create and alloc memory for index buffer
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));
    m_indexBuffer.release();
}
