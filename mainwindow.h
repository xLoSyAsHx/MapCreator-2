#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QBasicTimer>

#include "Model3D_4/model3d_4.h"


class SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;

class MainWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void initShaders();
    void initCube(float width);

private:
    Camera3D *m_camera;
    SkyBox *m_skybox;

    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_shaderProgramSkyBox;

    QVector2D m_mousePosition;

    QVector<SimpleObject3D*> m_objects;
    QVector<Group3D*> m_groups;
    QVector<Transformational*> m_transformObjects;

    // Animation
    QBasicTimer m_timer;
    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;

    Model3D_4 *m_model3dTest;
};

#endif // MAINWINDOW_H
