#ifndef LANDSCAPE_H
#define LANDSCAPE_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>


#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include "Model3D/node.h"
#include "Model3D/material.h"
#include "Model3D/vertexdata.h"

#include "transformational.h"

#include "landscapesculpttool.h"


struct Border2D {

    Border2D() :
        IsValid(false)
    {
    }

    Border2D(QVector2D topLeft,  QVector2D topRight,
             QVector2D downLeft, QVector2D downRight) :
        TopLeft(topLeft),   TopRight(topRight),
        DownLeft(downLeft), DownRight(downRight),
        IsValid(true)
    {
    }

    Border2D& operator *=(uint value) {
        TopLeft *= value;
        TopRight *= value;
        DownLeft *= value;
        DownRight *= value;

        return *this;
    }


    QVector2D TopLeft;
    QVector2D TopRight;
    QVector2D DownLeft;
    QVector2D DownRight;
    bool IsValid;
};




class Landscape : Transformational
{
public:
    Landscape(uint width, uint height, uint blockSize);

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override;

    void calculateNormals();
    void resetBuffers();
    Border2D refreshByLandscapeTool();
    Border2D refreshByLandscapeTool(QVector2D center, float size);

private:
    void clear();
    void fillIndexes(uint width, uint height);
    void fillVertexes(uint width, uint height, uint blockSize);

    QVector3D getNormalByFaceIndex(uint index) const;
    Border2D getSmallestBorderForTool(QVector2D center, float size) const;


    QOpenGLBuffer m_indexBuffer;
    QOpenGLBuffer m_vertexBuffer;

    QVector<VertexData> m_vertexes;
    QVector<uint> m_indexes;

    int m_width, m_length; // width - X. length - Z
    uint m_blockSize;


};

#endif // LANDSCAPE_H
