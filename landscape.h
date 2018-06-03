#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <functional>
#include <iostream>


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
#include "Model3D/material.h"
#include <QOpenGLTexture>

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

class LandscapeManager;


//using Mfunc = std::func
//using MFunc =  QVector4D (LandscapeManager::*MFunc)(); // MFunc - тип указателя

class Landscape : public Transformational
{
public:
    using Mfunc = std::function<QVector4D()>;

    Landscape(uint width, uint height, uint blockSize, Mfunc generateId);
    ~Landscape();

    struct PrimitivePointPositions {
        QVector3D pointA;
        QVector3D pointB;
        QVector3D pointC;
    };

    // Transformational interface
public:
    void rotate(const QQuaternion &rotation) override;
    void translate(const QVector3D &translation) override;
    void scale(const float scaleKoef) override;
    void setGlobalTransform(const QMatrix4x4 &matrix) override;
    void draw(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override;
    void objectPicking(QOpenGLShaderProgram *programm, QOpenGLFunctions *functions) override;

    QVector3D getPosition() const override;
    PrimitivePointPositions getTrianglePointGlobalPositions(
            int triangleId, QMatrix4x4 projectionMatrix);

    void addTexture(aiTextureType type, QOpenGLTexture *image);


    void calculateNormals();
    void resetBuffers();

    void setSculptToolUsage(bool isLandscapeSculptToolUse);
    void refreshByLandscapeTool();
    void refreshByLandscapeTool(QVector2D center, float size);

private:
    void clear();
    void fillIndexes(uint width, uint height);
    void fillVertexes(uint width, uint height, uint blockSize, Mfunc generateId);
    void fillTextureCoords(uint width, uint height);

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

            bool statusNoErr = true;
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

    Material m_landscapeMaterial;

    QOpenGLBuffer m_indexBuffer;
    QOpenGLBuffer m_vertexBuffer;

    QVector<VertexData> m_vertexes;
    QVector<uint> m_indexes;

    int m_width, m_length; // width - X. length - Z
    uint m_blockSize;

    QVector3D m_translate = QVector3D(0, 0, 0);


    bool m_isSculpToolUse = false;
};

#endif // LANDSCAPE_H
