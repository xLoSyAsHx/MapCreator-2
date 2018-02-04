#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLContext>

#include <QtMath>

#include "simpleobject3d.h"
#include "group3d.h"

#include "camera3d.h"
#include "skybox.h"

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent)
{
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

MainWindow::~MainWindow()
{
    delete m_camera;

    for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
        delete *it;

    for (auto it = m_groups.begin(); it != m_groups.end(); ++it)
        delete *it;

    for (auto it = m_transformObjects.begin(); it != m_transformObjects.end(); ++it)
        delete *it;
}

void MainWindow::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();

    float width = 8.0f;
    float height = 8.0f;
    float length = 8.0f;

    float step = 4.0f;

    // Group 1
    m_groups.append(new Group3D);
    //m_groups[0]->addObject(m_camera);
    for (float x = -width; x <= width; x += step) {

        for (float y = -height; y <= height; y += step) {

            for (float z = -length; z <= length; z += step) {
                initCube(2.0f);
                m_objects.last()->translate(QVector3D(x, y, z));
                m_groups.last()->addObject(m_objects.last());
            }
        }
    }
    m_groups[0]->translate(QVector3D(-12.0, 0.0f, 0.0f));

    // Group 2
    m_groups.append(new Group3D);
    for (float x = -width; x <= width; x += step) {

        for (float y = -height; y <= height; y += step) {

            for (float z = -length; z <= length; z += step) {
                initCube(2.0f);
                m_objects.last()->translate(QVector3D(x, y, z));
                m_groups.last()->addObject(m_objects.last());
            }
        }
    }
    m_groups[1]->translate(QVector3D(12.0, 0.0f, 0.0f));

    // Group 3
    m_groups.append(new Group3D);
    m_groups.last()->addObject(m_groups[0]);
    m_groups.last()->addObject(m_groups[1]);

    m_transformObjects.append(m_groups.last());

    m_skybox = new SkyBox(1000.0f, QImage(":/skybox.jpg"));

    //m_timer.start(30, this);
    m_model3dTest->loadFromFile("G:\\Programming\\Qt\\MapCreator\\9v.fbx");
    qDebug() << "asd";
}

void MainWindow::resizeGL(int w, int h)
{
    float aspect = w / qreal(h ? h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.01f, 2000.0f);
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Draw Skybox
    m_shaderProgramSkyBox.bind();
    // Set uniform values
    m_shaderProgramSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_shaderProgramSkyBox);
    m_skybox->draw(&m_shaderProgramSkyBox, context()->functions());

    m_shaderProgramSkyBox.release();


    // Draw objects
    m_shaderProgram.bind();
    // Set uniform values
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram.setUniformValue("u_lightPower", 1.0f);

    m_camera->draw(&m_shaderProgram);
    for (auto it = m_transformObjects.cbegin(); it != m_transformObjects.cend(); ++it)
        (*it)->draw(&m_shaderProgram, context()->functions());

    m_shaderProgram.release();



    //m_model3dTest->draw(&m_shaderProgram, context()->functions());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
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

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    update();
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    m_camera->translate(QVector3D(0.0f, 0.0f, 0.01 * event->delta()));
    update();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    for (int i = 0; i < m_objects.size(); ++i) {
        if (i % 2 == 0) {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleObject)));
        }
        else {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qSin(angleObject)));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleObject)));
        }
    }

    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, qSin(angleGroup1)));
    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 1.0f, -qSin(angleGroup1)));

    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleGroup2)));
    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 1.0f, -qCos(angleGroup2)));

    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleMain)));
    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleMain)));

    angleObject += M_PI / 180.0f;
    angleGroup1 += M_PI / 360;
    angleGroup2 -= M_PI / 360;
    angleMain += M_PI / 720;

    update();
}

void MainWindow::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "://vshader.vsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'vshader.vsh'");
        close();
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "://fshader.fsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'fshader.fsh'");
        close();
    }

    if (!m_shaderProgram.link()) {
        QMessageBox::about(this, "Error", "Can't link shader program");
        close();
    }


    // Skybox shaders
    if (!m_shaderProgramSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, "://skybox.vsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'skybox.vsh'");
        close();
    }

    if (!m_shaderProgramSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, "://skybox.fsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'skybox.fsh'");
        close();
    }

    if (!m_shaderProgramSkyBox.link()) {
        QMessageBox::about(this, "Error", "Can't link shader program");
        close();
    }
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

    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage("://cube.jpg")));
}

