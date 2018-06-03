#ifndef SYSTEM_H
#define SYSTEM_H


#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QScopedPointer>

#include <QBasicTimer>
#include <QTime>

#include "Model3D_4/model3d.h"
#include "landscape.h"
#include "pickingtexture.h"

#include "landscapemanager.h"
#include "landscapesculpttool.h"

#include "objectmanager.h"
#include "group3d.h"

#include "camera3d.h"
#include <QTimer>

class SimpleObject3D;
class Transformational;
class Camera3D;
class SkyBox;



class   System : public QOpenGLWidget
{
    Q_OBJECT

signals:
    void onObjectsPositionChanged(QVector3D shift);

public:
    System(QWidget *parent = 0);
    ~System();

    void onSculptToolStatusChanged();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void initShaders();


    struct MyLine {
        MyLine(System* s, QVector2D m_mousePosition) :
            indexBuffer(QOpenGLBuffer::IndexBuffer),
            vertexBuffer(QOpenGLBuffer::VertexBuffer)
        {
            QVector3D xzDirection = Camera3D::Instance().getDirection();
            xzDirection.setY(0.0f);

            QVector3D xzAxis = QVector3D::crossProduct(QVector3D(0, 1, 0), xzDirection);


            float aspect = s->size().width() / qreal(s->size().height() ? s->size().height() : 1);
            float oyCamAngle = m_mousePosition.y() / (float)s->size().height() * 45.0f - 22.5f;
            float oxCamAngle = m_mousePosition.x() / (float)s->size().width() * 45.0f * aspect - (45.0f * aspect / 2.0f);

            QVector3D camDir = Camera3D::Instance().getDirection();
            //ObjectManager::Instance().changeSelectionStatus(objId);

            QVector3D mouseDir;
            QQuaternion yRotation = QQuaternion::fromAxisAndAngle(xzAxis, oyCamAngle);
            mouseDir = (yRotation).rotatedVector(camDir);


            QQuaternion xzRotation = QQuaternion::fromAxisAndAngle(
                        QVector3D::crossProduct(mouseDir, xzAxis),
                        oxCamAngle).inverted();
            mouseDir = (xzRotation).rotatedVector(mouseDir);




            //QVector3D mouseDir = xzRotation.inverted().rotatedVector(Camera3D::Instance().getDirection());

            QVector<QVector3D> vertexes;
            QVector<uint> indexes;


            QVector3D v1 = Camera3D::Instance().getPosition();
            //QVector3D v2 = Camera3D::Instance().getPosition() + mouseDir * 1000;
            QVector3D v2 = Camera3D::Instance().getPosition() + Camera3D::Instance().getDirection() * 500;
            QVector3D v3 = v1;
            v3.setY(v2.y());

            QVector3D v4 = Camera3D::Instance().getPosition() + mouseDir * 1000;


            vertexes.push_back(v1);
            vertexes.push_back(v2);
            vertexes.push_back(v3);
            vertexes.push_back(v4);



            qDebug() << vertexes[0] << "      " << vertexes[1];


            indexes.push_back(0);
            indexes.push_back(1);

            indexes.push_back(1);
            indexes.push_back(2);


            indexes.push_back(2);
            indexes.push_back(0);


            indexes.push_back(0);
            indexes.push_back(3);

            indexBuffer.create();
            indexBuffer.bind();
            indexBuffer.allocate(indexes.constData(), sizeof(uint) * indexes.size());
            indexBuffer.release();

            vertexBuffer.create();
            vertexBuffer.bind();
            vertexBuffer.allocate(vertexes.constData(), sizeof(QVector3D) * vertexes.size());
            vertexBuffer.release();

        }

        void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions,
                  QMatrix4x4 &projectionMatrix)
        {
            QMatrix4x4 modelMatrix;
            modelMatrix.setToIdentity();


            // Bind buffers
            vertexBuffer.bind();
            indexBuffer.bind();

            program->setUniformValue("u_modelMatrix", modelMatrix);

            // Enable attribute arrays
            int loc_position = program->attributeLocation("a_position");
            program->enableAttributeArray(loc_position);



            // Set varying attribute "a_position"
            program->setAttributeBuffer(loc_position, GL_FLOAT, 0, 3, sizeof(QVector3D));

            // Draw
            functions->glDrawElements(GL_LINES, indexBuffer.size(), GL_UNSIGNED_INT, 0);


            // Release buffers
            vertexBuffer.release();
            indexBuffer.release();
        }

        QOpenGLBuffer indexBuffer;
        QOpenGLBuffer vertexBuffer;
    };

    MyLine* myLine = nullptr;

private:
    QTime m_frameTime;
    int m_frameCount;

    QSharedPointer<SkyBox> m_skybox;

    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_shaderProgramSkyBox;
    QOpenGLShaderProgram m_shaderProgramObjectPicking;
    QOpenGLShaderProgram m_sharedProgram_SelectedObjects;
    bool m_isObjectPicking = false;
    bool m_isLandscapePicking = false;

    QVector2D m_mousePosition;


    QTimer m_updateTimer;
    QTime m_updateLandscapeTime;
    float m_landscapeLastCheck;

    void renderScene();
    QVector4D renderSceneForGetObjectId();
    QVector4D renderSceneForGetLandscapeTriangleId();
};

#endif // SYSTEM_H
