#ifndef LANDSCAPE_H
#define LANDSCAPE_H


// Qt includes
#define uint unsigned int
#include <vector>
#include <algorithm>
#include <iostream>

#define qMax std::max
#define qMin std::min
#define qFloor 
#define qPow 


class QVector2D {
	float x, y;

public:
	QVector2D() {}
	QVector2D(float a, float b) :
		x(a), y(b)
	{
	}

	float x() const { return x; }
	float y() const { return y; }

	float length() {

	}

	QVector2D& operator *= (float value) {
		x *= value;
		y *= value;

		return *this;
	}

	QVector2D operator - (const QVector2D& vec) {

		return QVector2D(x - vec.x, y - vec.y);
	}

	friend QVector2D operator - (const QVector2D& vec1, const QVector2D& vec2) {

		return QVector2D(vec1.x - vec2.x, vec1.y - vec2.y);
	}

	QVector2D operator + (const QVector2D& vec) {

		return QVector2D(x + vec.x, y + vec.y);
	}

};

class QVector3D {
	float _x, _y, _z;

public:
	QVector3D(float a, float b, float c) : 
		_x(a), _y(b), _z(c)
	{
	}

	float x() const { return _x; }
	float y() const { return _y; }
	float z() const { return _z; }

	QVector3D& operator *= (float value) {
		_x *= value;
		_y *= value;
		_z *= value;

		return *this;
	}

	QVector3D& operator += (QVector3D & vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;


		return *this;
	}

	QVector3D operator - (QVector3D & vec) {


		return QVector3D(x - vec.x, y - vec.y, z - vec.z);
	}
};


struct IndexBorder2D {
	IndexBorder2D()
	{
	}

	IndexBorder2D(uint topLeft, uint downLeft) :
		TopLeft(topLeft), DownLeft(downLeft)
	{
	}

	uint TopLeft;
	uint DownLeft;
};


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

struct VertexData {
public:
	QVector3D position;
	QVector2D texCoord;
	QVector3D normal;
};

class Landscape
{
public:
    Landscape(uint width, uint height, uint blockSize);

    // Transformational interface
public:

    void calculateNormals();
    void resetBuffers();
    Border2D refreshByLandscapeTool(QVector2D center, float size);

private:
    void clear();
    void fillIndexes(uint width, uint height);
    void fillVertexes(uint width, uint height, uint blockSize);

    QVector3D getNormalByFaceIndex(uint index) const;
	QVector3D calculateNormalByIndex(uint index) const;
    Border2D getSmallestBorderForTool(QVector2D center, float size) const;
	uint getIndexByShiftFromCenter(int x, int z);

	IndexBorder2D getIndexBorderForToolBySize(QVector2D toolCenter, float size);
	std::pair<int, int> getStartAndXIndexes(const IndexBorder2D & border, const QVector2D & toolCenter, float size) const;
	void fillPerimeter(std::vector<uint>& downSide, std::vector<uint>& topSide,
		std::vector<uint> & indexesForUpdateVertexes,
		std::vector<uint> & indexForUpdateNormalsOnIntersects,
		QVector2D toolCenter, float size,
		bool isNeedToCheckBorderIntersects);

	void fillNormalsPerimeterWithoutBorderIntersects(std::vector<uint>& indexesForUpdateNormals,
		const QVector2D & toolCenter, float size);

	struct InformationForUpdate {
		InformationForUpdate(QVector2D toolCenter, float toolSize, int rowLength) :
			ToolCenter(toolCenter), ToolSize(toolSize), RowLength(rowLength), CanUpdate(false)
		{
		}

		int RowLength;

		float ToolSize;
		QVector2D ToolCenter;

		int StartIndex;
		int CurrentIndex;
		int PrevIndex;

		IndexBorder2D IndexBorder;

		std::vector<uint> IndexesForUpdateVertexes;
		std::vector<uint> IndexForUpdateNormalsOnIntersects;

		bool IsNeedToCheckBorderIntersects;
		bool CanUpdate;
	};

	void prepareInfoForUpdateOuterRing(InformationForUpdate & info);
	void prepareInfoForUpdateInnerCircle(InformationForUpdate & info);

	std::vector<uint> getCircleDownSide(InformationForUpdate info);
	std::vector<uint> getCircleTopSide(InformationForUpdate info);


	void updateVertexPositions(const std::vector<uint> & side, const std::vector<uint> & interiorSide, float toolStrength);
	void updateVertexesNormals(const std::vector<uint> & downSide, const std::vector<uint> & topSide);






	std::vector<VertexData> m_vertexes;
    std::vector<uint> m_indexes;

    int m_width, m_length; // width - X. length - Z
    uint m_blockSize;


};

#endif // LANDSCAPE_H
