#ifndef LANDSCAPE_H
#define LANDSCAPE_H


// Qt includes
#include <QString>
#include <QVector>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QPair>


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

struct IndexBorder2D {
    IndexBorder2D() :
        IsValid(false)
    {
    }

    IndexBorder2D(int topLeft, int downLeft, int topRight, int downRight) :
        TopLeft(topLeft), DownLeft(downLeft),
        TopRight(topRight), DownRight(downRight),
        IsValid(true)
    {
    }

    int TopLeft;
    int DownLeft;
    int TopRight;
    int DownRight;

    bool IsValid;
};


class Landscape : Transformational
{
public:
    Landscape(uint width, uint height, uint blockSize);
    ~Landscape();

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override;

    void calculateNormals();
    void resetBuffers();
    void refreshByLandscapeTool();
    void refreshByLandscapeTool(QVector2D center, float size);

private:
    void clear();
    void fillIndexes(uint width, uint height);
    void fillVertexes(uint width, uint height, uint blockSize);

    bool isCircleIntersectsLandscape(QVector2D circleCenter, float circleRadius);
    int getShiftedIndexForPointOnBorderIntersects(int index); // If point is located on intersects function return new index for vector (which is not located on intersects)
                                                              // Otherwise function return -1

    QVector3D getNormalByFaceIndex(uint index) const;
    QVector3D calculateNormalByIndex(uint index) const;

    IndexBorder2D getIndexBorderForToolBySize(QVector2D toolCenter, float size);
    QPair<int, int> getStartAndXIndexes(const IndexBorder2D & border,
                                        const QVector2D & toolCenter, float circleRadius) const;
    uint getIndexByShiftFromCenter(int x, int y) const;

    struct InformationForUpdate {
            InformationForUpdate(QVector2D toolCenter, float toolRadius, int rowLength) :
                ToolCenter(toolCenter), ToolRadius(toolRadius), RowLength(rowLength), CanUpdate(false)
            {
            }

            int RowLength;

            float ToolRadius;
            QVector2D ToolCenter;

            int StartIndex;
            int CurrentIndex;
            int PrevIndex;

            IndexBorder2D IndexBorder;
            uint LandscapeDownLeftIndex;

            QVector<uint> IndexesForUpdateVertexes;
            QVector<uint> IndexForUpdateNormalsOnIntersects;

            bool IsNeedToCheckBorderIntersects;
            bool CanUpdate;
        };


    void prepareInfoForUpdateOuterRing(InformationForUpdate & info);
    void prepareInfoForUpdateInnerCircle(InformationForUpdate & info);

    QVector<uint> getCircleDownSide(InformationForUpdate info);
    QVector<uint> getCircleTopSide(InformationForUpdate info);


    void updateVertexPositions(const QVector<uint> & side, const QVector<uint> & interiorSide, float toolStrength);
    void updateVertexesNormals(const QVector<uint> & side);

    void findAndUpdateVertexesOnBorderIntersects(InformationForUpdate const &info);
    QPair<int, int> getBordersOnIntersects(const InformationForUpdate & info,
                                           int low, int high, int step);

    QOpenGLBuffer m_indexBuffer;
    QOpenGLBuffer m_vertexBuffer;

    QVector<VertexData> m_vertexes;
    QVector<uint> m_indexes;

    int m_width, m_length; // width - X. length - Z
    uint m_blockSize;


};

#endif // LANDSCAPE_H
