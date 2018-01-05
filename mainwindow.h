#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

#include <QBasicTimer>


class SimpleObject3D;
class Transformational;
class Group3D;

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
    // Camera position z
    float m_z;

    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgramm;

    QVector2D m_mousePosition;
    QQuaternion m_rotation;

    QVector<SimpleObject3D*> m_objects;
    QVector<Group3D*> m_groups;
    QVector<Transformational*> m_transformObjects;

    // Animation
    QBasicTimer m_timer;
    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;
};

#endif // MAINWINDOW_H
