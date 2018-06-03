#include "skybox.h"

#include "simpleobject3d.h"

SkyBox::SkyBox(float width, const QImage &texture)
{
    float width_div_2 = width / 2;
    QVector<VertexData> vertexes;


    //front, right, top, back, left, down

    // Top side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(2.0f / 4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f / 4.0f, 1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, -1.0f, 0.0f)));


    // Down side
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f / 4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(2.0f / 4.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 1.0f, 0.0f)));

    // Left side
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 2.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 1.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    // Right side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0f / 4.0f, 2.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0f / 4.0f, 1.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

    // Front side
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, -1.0f)));

    // Back side
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 2.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f / 4.0f, 1.0f / 3.0f), QVector3D(0.0f, 0.0f, 1.0f)));


    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4){
        indexes.append(i + 0);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 3);
        indexes.append(i + 1);
    }

    m_cube = new SimpleObject3D(vertexes, indexes, texture);
}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::rotate(const QQuaternion &rotation)
{
    (void)rotation;
}

void SkyBox::translate(const QVector3D &translation)
{
    (void)translation;
}

void SkyBox::scale(const float scaleKoef)
{
    (void)scaleKoef;
}

void SkyBox::setGlobalTransform(const QMatrix4x4 &matrix)
{
    (void)matrix;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program, functions);
}

QVector3D SkyBox::getPosition() const
{
    return QVector3D(0, 0, 0);
}
