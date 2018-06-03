#include "system.h"

#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QTime>

#include <QtMath>

#include "simpleobject3d.h"
#include "group3d.h"

#include "camera3d.h"
#include "skybox.h"


#include "landscapemanager.h"

#include <GL/glu.h>
#include <QTimer>
#include <QCursor>


System::System(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    Camera3D::Instance().translate(QVector3D(0.0f, 5.0f, -5.0f));
}

System::~System()
{

}

void System::onSculptToolStatusChanged()
{
    m_isLandscapePicking = !m_isLandscapePicking;
}

void System::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();


    m_skybox.reset(new SkyBox(1000.0f, QImage(":/skybox.jpg")));

    //m_model3dTest = new Model3D();
    //m_model3dTest->translate(QVector3D(0, 0, 0));
    //m_model3dTest->loadFromFile("G:\\Programming\\Qt\\MapCreator\\9v.fbx");

    m_frameTime.start();
    m_frameCount = 0;
    m_updateLandscapeTime.start();


    //m_updateTimer.setInterval(20);
    //m_updateTimer.connect(&m_updateTimer, QTimer::timeout, this, [this](){ update(); });
    //m_updateTimer.start(50);

}

void System::resizeGL(int w, int h)
{
    float aspect = w / qreal(h ? h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.01f, 2000.0f);
}

void System::paintGL()
{
    if (m_isObjectPicking)
    {
        QVector4D objId = renderSceneForGetObjectId();
        //qDebug() << "ObjId: " << objId;
        //qDebug() << objId.x() * 255 << " " << objId.y() * 255 <<  " " << objId.z() * 255 << " " << objId.w() * 255;
        ObjectManager::Instance().changeSelectionStatus(objId);
        m_isObjectPicking = false;
    }
    else if (m_isLandscapePicking)
    {
        QVector4D triangleId = renderSceneForGetLandscapeTriangleId();


        QVector3D xzDirection = Camera3D::Instance().getDirection();
        xzDirection.setY(0.0f);

        QVector3D xzAxis = QVector3D::crossProduct(QVector3D(0, 1, 0), xzDirection);


        float aspect = size().width() / qreal(size().height() ? size().height() : 1);
        float oyCamAngle = m_mousePosition.y() / (float)size().height() * 45.0f - 22.5f;
        float oxCamAngle = m_mousePosition.x() / (float)size().width() * 45.0f * aspect - (45.0f * aspect / 2.0f);

        QVector3D camDir = Camera3D::Instance().getDirection();
        //ObjectManager::Instance().changeSelectionStatus(objId);

        QVector3D mouseDir;
        QQuaternion yRotation = QQuaternion::fromAxisAndAngle(xzAxis, oyCamAngle);
        mouseDir = (yRotation).rotatedVector(camDir);


        QQuaternion xzRotation = QQuaternion::fromAxisAndAngle(
                    QVector3D::crossProduct(mouseDir, xzAxis),
                    oxCamAngle).inverted();
        mouseDir = (xzRotation).rotatedVector(mouseDir);

        mouseDir= mouseDir * 100;

        LandscapeManager::Instance().updateLandscapeToolPosition(mouseDir, triangleId, m_projectionMatrix);

    }

    renderScene();



    m_sharedProgram_SelectedObjects.bind();

    m_sharedProgram_SelectedObjects.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    Camera3D::Instance().draw(&m_sharedProgram_SelectedObjects);

    //if (myLine != nullptr)
        //myLine->draw(&m_sharedProgram_SelectedObjects,context()->functions(),m_projectionMatrix);

    m_sharedProgram_SelectedObjects.release();





    ++m_frameCount;

    if (m_frameTime.elapsed() >= 1000)
    {
        double fps = m_frameCount / ((double)m_frameTime.elapsed()/1000.0);
        m_frameTime.restart();
        m_frameCount = 0;
        //m_frameTime.start();
        //qDebug() << fps;
    }
}

void System::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::question( this,  QApplication::applicationName(), tr(" Do you want to close programm ? "),
                                 QMessageBox::Yes | QMessageBox::No , QMessageBox::No );
    if (ret == QMessageBox::No)
        event->ignore();
    else
        close();
}

void System::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        m_mousePosition = QVector2D(event->localPos());
    }
    else if (event->buttons() == Qt::RightButton)
    {
        m_mousePosition = QVector2D(event->localPos());
        update();
    }

    event->accept();
}

void System::mouseMoveEvent(QMouseEvent *event)
{
    static QVector3D xzLastAxis;

    QVector2D lastMousePosition = m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    if (event->buttons() == Qt::RightButton)
        {
           update();
           return;
        }

    if (event->buttons() != Qt::LeftButton) return;

    float xzAngle = (m_mousePosition.x() - lastMousePosition.x()) / 5.0f;
    float yAngle = (m_mousePosition.y() - lastMousePosition.y()) / 5.0f;

    QVector3D xzDirection = Camera3D::Instance().getDirection();
    bool isDirectionYPositive = xzDirection.y() >= 0;
    xzDirection.setY(0.0f);

    QVector3D xzAxis = QVector3D::crossProduct(QVector3D(0, 1, 0), xzDirection);

    if (qAbs(xzAxis.x()) < 0.01 && qAbs(xzAxis.z()) < 0.01)
    {
        xzAxis = xzLastAxis;

        if (isDirectionYPositive && yAngle > 0 ||
                !isDirectionYPositive && yAngle < 0)
        {
            return;
        }
    }


    QQuaternion xzRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), xzAngle);
    QQuaternion yRotation = QQuaternion::fromAxisAndAngle(xzAxis, yAngle).inverted();

    xzLastAxis = xzAxis;

    Camera3D::Instance().rotate(xzRotation * yRotation);
    Camera3D::Instance().lookAt();

    update();

}

void System::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_isObjectPicking = true;
    m_mousePosition = QVector2D(event->localPos());

    if (myLine != nullptr)
        delete myLine;

    myLine = new MyLine(this, m_mousePosition);

    update();
}

void System::keyPressEvent(QKeyEvent *event)
{
    int pressedKey = event->key();

    // Move objects
    if (QApplication::keyboardModifiers() && Qt::ControlModifier)
    {
        ObjectManager& objMngr = ObjectManager::Instance();
        int numOfSelectedObjs = objMngr.getNumOfSelectedObjects();

        if (numOfSelectedObjs == 0)
            return;

        QVector3D vec = Camera3D::Instance()
                .getRotation()
                .rotatedVector(QVector3D(0, 0, -1));
        vec.setY(0.0f);

        QVector3D resultShift;

        if (pressedKey == Qt::Key_W)
        {
            resultShift += vec;
        }
        else if (pressedKey == Qt::Key_S)
        {
            resultShift -= vec;
        }

        if (pressedKey == Qt::Key_A)
        {
            resultShift -= QVector3D::crossProduct(vec, QVector3D(0, 1, 0));
        }
        else if (pressedKey == Qt::Key_D)
        {
            resultShift += QVector3D::crossProduct(vec, QVector3D(0, 1, 0));
        }

        if (pressedKey == Qt::Key_Q)
        {
            resultShift += QVector3D(0, 0.3, 0);
        }
        else if (pressedKey == Qt::Key_E)
        {
            resultShift += QVector3D(0, -0.3, 0);
        }


        objMngr.translateSelectedObjects(resultShift);
        emit onObjectsPositionChanged(resultShift);
    }
    else
    {
        QVector3D vec = Camera3D::Instance().getRotation()
                .rotatedVector(QVector3D(0, 0, -1));

        if (pressedKey == Qt::Key_W)
        {
            Camera3D::Instance().translate(vec);
        }
        else if (pressedKey == Qt::Key_S)
        {
            Camera3D::Instance().translate(QVector3D(0, 0, 0) - vec);
        }

        if (pressedKey == Qt::Key_A)
        {
            Camera3D::Instance().translate(QVector3D(0, 0, 0) - QVector3D::crossProduct(vec, QVector3D(0, 1, 0)));
        }
        else if (pressedKey == Qt::Key_D)
        {
            Camera3D::Instance().translate(QVector3D::crossProduct(vec, QVector3D(0, 1, 0)));
        }

        if (pressedKey == Qt::Key_Q)
        {
            Camera3D::Instance().translate(QVector3D(0, 0.3, 0));
        }
        else if (pressedKey == Qt::Key_E)
        {
            Camera3D::Instance().translate(QVector3D(0, -0.3, 0));
        }


        qDebug() << m_updateLandscapeTime.elapsed();
        if (pressedKey == Qt::Key_R && 200 < m_updateLandscapeTime.elapsed())
        {
            //LandscapeSculptTool::Instance().setCenter(QVector2D(0, 0));
            LandscapeManager::Instance().refreshByLandscapeTool();
            m_updateLandscapeTime.restart();
        }


    }

    update();
}

void System::wheelEvent(QWheelEvent *event)
{
    Camera3D& camera = Camera3D::Instance();
    camera.translate(camera.getDirection() * event->delta() * 0.05);
    update();
}


void System::initShaders()
{
    //bool status = m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/landscape.fsh");

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




    // Object picking shader
    if (!m_shaderProgramObjectPicking.addShaderFromSourceFile(QOpenGLShader::Vertex, "://object_picking.vsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'object_picking.vsh'");
        close();
    }

    if (!m_shaderProgramObjectPicking.addShaderFromSourceFile(QOpenGLShader::Fragment, "://object_picking.fsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'object_picking.fsh'");
        close();
    }

    if (!m_shaderProgramObjectPicking.link()) {
        QMessageBox::about(this, "Error", "Can't link shader program");
        close();
    }



    // Selection cubes shader
    if (!m_sharedProgram_SelectedObjects.addShaderFromSourceFile(QOpenGLShader::Vertex, "://object_selection_cubes.vsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'object_selection_cubes.vsh'");
        close();
    }

    if (!m_sharedProgram_SelectedObjects.addShaderFromSourceFile(QOpenGLShader::Fragment, "://object_selection_cubes.fsh")) {
        QMessageBox::about(this, "Error", "Can't add shader from source file 'object_selection_cubes.fsh'");
        close();
    }

    if (!m_sharedProgram_SelectedObjects.link()) {
        QMessageBox::about(this, "Error", "Can't link shader program");
        close();
    }
}

void System::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Skybox
    m_shaderProgramSkyBox.bind();
    // Set uniform values
    m_shaderProgramSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    Camera3D::Instance().draw(&m_shaderProgramSkyBox);
    m_skybox->draw(&m_shaderProgramSkyBox, context()->functions());

    m_shaderProgramSkyBox.release();


    // Draw objects
    m_shaderProgram.bind();
    // Set uniform values
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram.setUniformValue("u_lightPower", 1.0f);

    Camera3D::Instance().draw(&m_shaderProgram);
    ObjectManager::Instance().draw(&m_shaderProgram, context()->functions());
    LandscapeManager::Instance().draw(&m_shaderProgram, context()->functions(), m_isLandscapePicking);

    m_shaderProgram.release();


    // Draw selection cubes
    m_sharedProgram_SelectedObjects.bind();

    m_sharedProgram_SelectedObjects.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    Camera3D::Instance().draw(&m_sharedProgram_SelectedObjects);

    ObjectManager::Instance().drawSelectionCubes(&m_sharedProgram_SelectedObjects, context()->functions());

    m_sharedProgram_SelectedObjects.release();
}

QVector4D System::renderSceneForGetObjectId()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PickingTexture pickingTexture;
    bool isSuccess = pickingTexture.Init(size().width(), size().height(),context()->functions());

    if (isSuccess == false)
        return QVector4D(0, 0, 0, 0);

    pickingTexture.EnableWriting(context()->functions());

    m_shaderProgramObjectPicking.bind();
    // Set uniform values
    m_shaderProgramObjectPicking.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    Camera3D::Instance().draw(&m_shaderProgramObjectPicking);
    ObjectManager::Instance().objectPicking(&m_shaderProgramObjectPicking, context()->functions());
    //LandscapeManager::Instance().objectPicking(&m_shaderProgramObjectPicking, context()->functions());

    m_shaderProgramObjectPicking.release();

    m_isObjectPicking = false;
    pickingTexture.DisableWriting(context()->functions());


    return pickingTexture.ReadPixel(
                m_mousePosition.x(), m_mousePosition.y(), context()->functions());
}

QVector4D System::renderSceneForGetLandscapeTriangleId()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PickingTexture pickingTexture;
    bool isSuccess = pickingTexture.Init(size().width(), size().height(),context()->functions());

    if (isSuccess == false)
        return QVector4D(0, 0, 0, 0);

    pickingTexture.EnableWriting(context()->functions());

    m_shaderProgramObjectPicking.bind();
    // Set uniform values
    m_shaderProgramObjectPicking.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    Camera3D::Instance().draw(&m_shaderProgramObjectPicking);
    LandscapeManager::Instance().objectPicking(&m_shaderProgramObjectPicking, context()->functions());

    m_shaderProgramObjectPicking.release();

    m_isObjectPicking = false;
    pickingTexture.DisableWriting(context()->functions());


    return pickingTexture.ReadPixel(
                m_mousePosition.x(), m_mousePosition.y(), context()->functions());
}


