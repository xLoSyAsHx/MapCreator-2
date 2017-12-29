#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent),
    m_texture(nullptr),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(1.0f);
}

void MainWindow::resizeGL(int w, int h)
{
    float aspect = w / qreal(h ? h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.1f, 10.0f);
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0.0f, 0.0f, -5.0f);
    modelViewMatrix.rotate(m_rotation);

    m_texture->bind(0);

    //m_shaderProgramm.bind();
    m_shaderProgramm.setUniformValue("MVPMatrix", m_projectionMatrix * modelViewMatrix);
    m_shaderProgramm.setUniformValue("qt_Texture0", 0);


    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    int offset = 0;

    int location = m_shaderProgramm.attributeLocation("a_position");
    m_shaderProgramm.enableAttributeArray(location);
    m_shaderProgramm.setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    location = m_shaderProgramm.attributeLocation("a_textcoord0");
    m_shaderProgramm.enableAttributeArray(location);
    m_shaderProgramm.setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));



    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "In close event\n";

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this,  QApplication::applicationName(), tr(" Do you want to close programm ? "),
                                 QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
    if (ret == QMessageBox::No)
        event->ignore();
    else
        close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        m_mousePosition = QVector2D(event->localPos());
    }

    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{


    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0f);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();
}

void MainWindow::initShaders()
{
    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Vertex, "://vshader.vsh"))
        close();

    if (!m_shaderProgramm.addShaderFromSourceFile(QOpenGLShader::Fragment, "://fshader.fsh"))
        close();

    if (m_shaderProgramm.link())
        close();

    if (!m_shaderProgramm.bind())
        close();

}

void MainWindow::initCube(float width)
{
    float width_div_2 = width / 2;
    QVector<VertexData> vertexes;


    //front, right, top, back, left, down

    // Top side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));


    // Down side
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f)));

    // Left side
    /*
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
*/
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

    // Right side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    // Front side
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    // Back side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, -1.0f)));


    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4){
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    // Create and fill vertex buffer and then -> release
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(VertexData));
    m_vertexBuffer.release();

    // Create and fill index buffer and then -> release
    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(QImage("://cube.jpg").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}
































