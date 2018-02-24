#include "landscape.h"

Landscape::Landscape(uint width, uint height, uint blockSize) :
    m_width(width), m_length(height), m_blockSize(blockSize)
{

    fillIndexes(width, height);
    fillVertexes(width, height, blockSize);
}


void Landscape::calculateNormals()
{
    for (int i = 0; i < m_length + 1; ++i) {
        for (int j = 0; i < m_width + 1; ++j) {
            QVector3D normal(0, 0, 0);

			std::vector<int> faceIndexes = {
                (i - 1) * (m_width * 2) + j * 2 - 1, // Top left
                (i - 1) * (m_width * 2) + j * 2, // Top middle
                (i - 1) * (m_width * 2) + j * 2 + 1, // Top right
                i * (m_width * 2) + j * 2 - 2, // Down left
                i * (m_width * 2) + j * 2 - 1, // Down middle
                i * (m_width * 2) + j * 2 // Down right
            };


            for (uint k = 0; k < 6; ++k) {
                if (faceIndexes[k] >= 0 && faceIndexes[k] < m_width * m_length * 2)
                    normal += getNormalByFaceIndex(faceIndexes[k]);
            }

            //normal.normalize();
            m_vertexes[i * (m_width + 1) + j].normal = normal;
        }
    }
}

void Landscape::resetBuffers()
{
    

}

void Landscape::clear()
{
    m_vertexes.clear();
    m_indexes.clear();
    m_width = -1;
    m_length = -1;
}

void Landscape::fillIndexes(uint width, uint height)
{
    m_indexes.resize(width * height * 2 * 3);

    uint lastAddIndex = 0;
    uint plusShift = width + 1;

    // Form even row (from left to right)
    for (uint j = 0; j < height; ++j) {
        for (uint i = 0; i < width; ++i) {
            uint pos = j * (width * 2 * 3) + 6 * i;

            m_indexes[pos] = lastAddIndex;
            m_indexes[pos + 1] = lastAddIndex + plusShift;
            m_indexes[pos + 2] = lastAddIndex + 1;


            m_indexes[3 + pos] = lastAddIndex + 1;
            m_indexes[3 + pos + 1] = lastAddIndex + plusShift;
            m_indexes[3 + pos + 2] = lastAddIndex + plusShift + 1;


            ++lastAddIndex;
        }

        ++lastAddIndex;
    }
}

void Landscape::fillVertexes(uint width, uint height, uint blockSize)
{
    m_vertexes.resize((width + 1) * (height + 1));

    QVector3D halfLensOfLandscape(width * blockSize / 2, height * blockSize / 2, 0);

    for (uint i = 0; i < height + 1; ++i) {
        for (uint j = 0; j < width + 1; ++j) {
            m_vertexes[i * (width + 1) + j].position = QVector3D(blockSize * j, blockSize * i, 0) - halfLensOfLandscape;
        }
    }
}

QVector3D Landscape::getNormalByFaceIndex(uint index) const
{
    uint ind1 = m_indexes[3 * index];
    uint ind2 = m_indexes[3 * index + 1];
    uint ind3 = m_indexes[3 * index + 2];

    return QVector3D(1, 1, 1);
}

QVector3D Landscape::calculateNormalByIndex(uint index) const
{
	QVector3D normal(0, 0, 0);

	int i = index / (m_width + 1);
	int j = index % (m_width + 1);

	std::vector<int> faceIndexes = {
		(i - 1) * (m_width * 2) + j * 2 - 1, // Top left
		(i - 1) * (m_width * 2) + j * 2, // Top middle
		(i - 1) * (m_width * 2) + j * 2 + 1, // Top right
		i * (m_width * 2) + j * 2 - 2, // Down left
		i * (m_width * 2) + j * 2 - 1, // Down middle
		i * (m_width * 2) + j * 2 // Down right
	};


	for (uint k = 0; k < 6; ++k) {
		if (faceIndexes[k] >= 0 && faceIndexes[k] < m_width * m_length * 2)
			normal += getNormalByFaceIndex(faceIndexes[k]);
	}

	return normal;
}


Border2D Landscape::refreshByLandscapeTool(QVector2D center, float size)
{
	if (isToolIntersectsThisPart() == false)
		return;

	QVector2D toolCenter(0, 0);
	float toolFalloff; // From 0 to 1
	float toolStrength;

	std::vector<uint> indexesForUpdateNormals;
	std::vector<uint> indexesForUpdateVertexes;
	std::vector<uint> indexForUpdateNormalsOnIntersects;

	InformationForUpdate infoForUpdate(toolCenter, size, m_width + 1);

	prepareInfoForUpdateOuterRing(infoForUpdate);
	std::vector<uint> downSide = getCircleDownSide(infoForUpdate);
	std::vector<uint> topSide = getCircleTopSide(infoForUpdate);

	

	//fillPerimeter(downSide, topSide, indexesForUpdateVertexes, indexForUpdateNormalsOnIntersects,
	//	toolCenter, size, true);

	//fillNormalsPerimeterWithoutBorderIntersects(indexesForUpdateNormals,
	//	toolCenter, size);

	if (isInnerCircleOfToolIntersectsThisPart() == false)
		infoForUpdate.CanUpdate = false;
	else
		prepareInfoForUpdateInnerCircle(infoForUpdate);

	infoForUpdate.ToolSize = size / 2;
	infoForUpdate.IsNeedToCheckBorderIntersects = false;
	std::vector<uint> downInteriorSide = getCircleDownSide(infoForUpdate);
	std::vector<uint> topInteriorSide = getCircleDownSide(infoForUpdate);

	// startIndex должен быть на той же строке на которой был при расчёте внешнего кольца
	//fillPerimeter(downInteriorSide, topInteriorSide, indexesForUpdateVertexes, indexForUpdateNormalsOnIntersects,
	//	toolCenter, size * toolFalloff, false);



	// Fill Down side
	updateVertexPositions(downSide, downInteriorSide, toolStrength);

	// Fill Top side
	updateVertexPositions(topSide, topInteriorSide, toolStrength);

	// Update vertex positions on intersects
	auto isInsideCircle = [squareMaxLength = qPow(size * toolFalloff, 2), toolCenter = toolCenter](VertexData &v) {
		return squareMaxLength <=
			qPow(v.position.x() - toolCenter.x, 2) +
			qPow(v.position.z() - toolCenter.y, 2);
	};

	for (int i = 0; i < indexesForUpdateVertexes.size(); ++i)
	{
		VertexData & v = m_vertexes[indexesForUpdateVertexes[i]];

		v.position += isInsideCircle(v) ? toolStrength : getKoefDependsFromLen();
	}


	// Пройтись по всем массивам индексов и обновить нормали
	updateVertexesNormals(downSide, topSide);

	for (int index = 0; index < indexesForUpdateNormals.size(); ++index) // All indexes out of circle (without intersects with border)
		m_vertexes[index].normal = calculateNormalByIndex(index);


	// Определить с какого места и сколько VertexDatas надо обновить в вершинном буфере
	



	// Обновить данные в вершинном буфере



    //return getSmallestBorderForTool(center, size);
}

Border2D Landscape::getSmallestBorderForTool(QVector2D center, float size) const
{
    Border2D border(QVector2D(-m_width / 2, -m_length / 2), // TopLeft
                    QVector2D(m_width / 2 + (m_width % 2), -m_length / 2), // TopRight
                    QVector2D(-m_width / 2, m_length / 2 + (m_length % 2)), // DownLeft
                    QVector2D(m_width / 2 + (m_width % 2), m_length / 2 + (m_length % 2))); // DownRight

    border *= m_blockSize;

    SmallestBorderForToolCalculator(center, size, m_blockSize).calculate(border);
    return border;
}

uint Landscape::getIndexByShiftFromCenter(int x, int z)
{
	return uint();
}

IndexBorder2D Landscape::getIndexBorderForToolBySize(QVector2D toolCenter, float size)
{
	float halfSize = size / 2;

	// Определить примерный прямоугольник, в котором находится центр
	int leftX = qMax(qFloor(toolCenter.x - halfSize) + 1, -m_width / 2); // 41.2 -> 41
	int rightX = qMin(qFloor(toolCenter.x + halfSize), m_width / 2 + m_width % 2);
	int topZ = qMax(qFloor(toolCenter.y - halfSize) + 1, -m_length / 2);
	int downZ = qMin(qFloor(toolCenter.y + halfSize), m_length / 2 + m_length % 2);
	int numIndexesInRow = rightX - leftX + 1;
	int numIndexesInColumn = downZ - topZ + 1;

	// Определить верхнюю левую и нижнюю правую точки для перерасчёта координат и нормалей
	return IndexBorder2D(getIndexByShiftFromCenter(leftX, topZ),
							getIndexByShiftFromCenter(leftX, topZ));
}

std::pair<int, int> Landscape::getStartAndXIndexes(const IndexBorder2D & border, const QVector2D & toolCenter, float size) const
{
	uint numOfRows = (border.DownLeft - border.TopLeft) / (m_width + 1) + 1;

	int startIndex = -1;

	//check (x_index + 1_row), (x_index), (x_index - 1_row) and (x_index - 2_row)
	uint x_index = border.TopLeft + (numOfRows / 2) * (m_width + 1);
	float squareMaxLength = qPow(size, 2);
	for (int i = -1; i < 3; ++i) {
		uint index = x_index - i * (m_width + 1);

		if (qPow(size, 2) <=
			qPow(m_vertexes[index].position.x() - toolCenter.x, 2) +
			qPow(m_vertexes[index].position.z() - toolCenter.y, 2))
		{
			startIndex = index;
			break;
		}
	}

	return std::make_pair(startIndex, x_index);
}

void Landscape::fillPerimeter(std::vector<uint>& downSide, std::vector<uint>& topSide, 
	std::vector<uint> & indexesForUpdateVertexes,
	std::vector<uint> & indexForUpdateNormalsOnIntersects,
	QVector2D toolCenter, float size,
	bool isNeedToCheckBorderIntersects)
{
	// Prepare necessary variables for fill perimeter
	IndexBorder2D exactIndexBorder = getIndexBorderForToolBySize(toolCenter, size);
	int rowLength = m_width + 1;
	
	std::pair<int, int> startAndX_indexes = getStartAndXIndexes(exactIndexBorder, toolCenter, size);
	int startIndex = startAndX_indexes.first;
	int currentIndex;
	int prevIndex;

	if (startIndex == -1)
		startIndex = currentIndex = prevIndex = startAndX_indexes.second + 1;
	else
		currentIndex = prevIndex = startIndex;



	auto isInsideCircle = [squareMaxLength = qPow(size, 2), toolCenter = toolCenter](VertexData &v) {
		return squareMaxLength <=
			qPow(v.position.x() - toolCenter.x, 2) +
			qPow(v.position.z() - toolCenter.y, 2);
	};

	// ########## Fill Down Side ##########

	// Fill DownLeft Side
	while (currentIndex < exactIndexBorder.DownLeft + m_width + 1) {

		if (isInsideCircle(m_vertexes[currentIndex]) == true)
		{
			if (isNeedToCheckBorderIntersects)
			{
				bool isLeftLandBorderIntersects = currentIndex % rowLength == 0;
				bool isDownLandBorderIntersects = currentIndex >= exactIndexBorder.DownLeft;

				if (isLeftLandBorderIntersects || isDownLandBorderIntersects)
				{
					indexesForUpdateVertexes.push_back(currentIndex);
					indexForUpdateNormalsOnIntersects.push_back(currentIndex);


					uint indexForAdd = currentIndex;
					if (isLeftLandBorderIntersects)
						++indexForAdd;

					if (isDownLandBorderIntersects)
						indexForAdd -= m_width + 1;

					downSide.push_back(indexForAdd);
				}
				else 
				{
					downSide.push_back(currentIndex);
				}
			}
			else 
			{
				downSide.push_back(currentIndex);
			}

			currentIndex += m_width + 1;
		}
		else {
			++currentIndex;
		}
	}

	// Find the most right index in the most down row
	currentIndex -= m_width;
	while (isInsideCircle(m_vertexes[currentIndex])) {
		++currentIndex;
	}

	// Prepare for fill DownRight Side
	prevIndex = currentIndex - 1;

	// Fill DownRight Side
	while (currentIndex > startIndex) {

		if (isInsideCircle(m_vertexes[currentIndex]) == false)
		{
			if (isNeedToCheckBorderIntersects)
			{
				bool isRightLandBorderIntersects = currentIndex % rowLength == m_width;
				bool isDownLandBorderIntersects = currentIndex >= exactIndexBorder.DownLeft;

				if (isRightLandBorderIntersects || isDownLandBorderIntersects)
				{
					indexesForUpdateVertexes.push_back(prevIndex);
					indexForUpdateNormalsOnIntersects.push_back(prevIndex);


					uint indexForAdd = prevIndex;
					if (isRightLandBorderIntersects)
						--indexForAdd;

					if (isDownLandBorderIntersects)
						indexForAdd -= m_width + 1;

					downSide.push_back(indexForAdd);
				}
				else 
				{
					downSide.push_back(prevIndex);
				}
			}
			else 
			{
				downSide.push_back(prevIndex);
			}

			prevIndex = currentIndex - (m_width + 1);
			currentIndex -= m_width + 1;
		}
		else {
			prevIndex = currentIndex;
			++currentIndex;
		}
	}

	// ########## Fill Top Side ##########

	// Prepare for fill TopLeft Side
	currentIndex = startIndex - (m_width + 1);

	// Fill TopLeft Side
	while (currentIndex > exactIndexBorder.TopLeft) {

		if (isInsideCircle(m_vertexes[currentIndex]) == true) {
			if (isNeedToCheckBorderIntersects)
			{
				bool isLeftLandBorderIntersects = currentIndex % rowLength == 0;
				bool isTopLandBorderIntersects = currentIndex < rowLength;

				if (isLeftLandBorderIntersects || isTopLandBorderIntersects)
				{
					indexesForUpdateVertexes.push_back(currentIndex);
					indexForUpdateNormalsOnIntersects.push_back(currentIndex);


					uint indexForAdd = currentIndex;
					if (isLeftLandBorderIntersects)
						++indexForAdd;

					if (isTopLandBorderIntersects)
						indexForAdd += m_width + 1;

					topSide.push_back(indexForAdd);
				}
				else
				{
					topSide.push_back(currentIndex);
				}
			}
			else 
			{
				topSide.push_back(currentIndex);
			}

			currentIndex -= m_width + 1;
		}
		else {
			++currentIndex;
		}
	}

	// Find the most right index in the most down row
	currentIndex += rowLength;
	while (isInsideCircle(m_vertexes[currentIndex])) {
		++currentIndex;
	}

	// Prepare for fill DownRight Side
	prevIndex = currentIndex - 1;

	// Fill TopRight Side
	while (currentIndex < startIndex) {

		if (isInsideCircle(m_vertexes[currentIndex]) == false) {
			if (isNeedToCheckBorderIntersects)
			{
				bool isRightLandBorderIntersects = currentIndex % rowLength == m_width;
				bool isTopLandBorderIntersects = currentIndex < rowLength;

				if (isRightLandBorderIntersects || isTopLandBorderIntersects)
				{
					indexesForUpdateVertexes.push_back(prevIndex);
					indexForUpdateNormalsOnIntersects.push_back(prevIndex);


					uint indexForAdd = prevIndex;
					if (isRightLandBorderIntersects)
						--indexForAdd;

					if (isTopLandBorderIntersects)
						indexForAdd += m_width + 1;

					topSide.push_back(indexForAdd);
				}
				else
				{
					topSide.push_back(prevIndex);
				}
			}
			else 
			{
				topSide.push_back(prevIndex);
			}

			currentIndex += m_width + 1;
		}
		else {
			prevIndex = currentIndex;
			++currentIndex;
		}
	}

}

void Landscape::fillNormalsPerimeterWithoutBorderIntersects(std::vector<uint>& indexesForUpdateNormals,
	const QVector2D & toolCenter, float size)
{
	IndexBorder2D border = getIndexBorderForToolBySize(toolCenter, size + 1);
}

void Landscape::prepareInfoForUpdateOuterRing(InformationForUpdate & info)
{
	info.IndexBorder = getIndexBorderForToolBySize(info.ToolCenter, info.ToolSize);
	int rowLength = m_width + 1;

	std::pair<int, int> startAndX_indexes = getStartAndXIndexes(info.IndexBorder, info.ToolCenter, info.ToolSize);
	info.StartIndex = startAndX_indexes.first;

	if (info.StartIndex == -1)
		info.StartIndex = info.CurrentIndex = info.PrevIndex = startAndX_indexes.second + 1;
	else
		info.CurrentIndex = info.PrevIndex = info.StartIndex;
}

void Landscape::prepareInfoForUpdateInnerCircle(InformationForUpdate & info)
{
	info.IndexBorder = getIndexBorderForToolBySize(info.ToolCenter, info.ToolSize);

	if (info.ToolSize == 0.0f) {
		info.CurrentIndex = -1;
		return;
	}

	
}

std::vector<uint> Landscape::getCircleDownSide(InformationForUpdate info)
{
	if (info.CanUpdate == false)
		return std::vector<uint>();

	auto isInsideCircle = [squareMaxLength = qPow(info.ToolSize, 2), toolCenter = info.ToolCenter](VertexData &v) {
		return squareMaxLength <=
			qPow(v.position.x() - toolCenter.x, 2) +
			qPow(v.position.z() - toolCenter.y, 2);
	};

	std::vector<uint> downSide;

	// Fill DownLeft Side
	while (info.CurrentIndex < info.IndexBorder.DownLeft + m_width + 1) {

		if (isInsideCircle(m_vertexes[info.CurrentIndex]) == true)
		{
			if (info.IsNeedToCheckBorderIntersects)
			{
				bool isLeftLandBorderIntersects = info.CurrentIndex % info.RowLength == 0;
				bool isDownLandBorderIntersects = info.CurrentIndex >= info.IndexBorder.DownLeft;

				if (isLeftLandBorderIntersects || isDownLandBorderIntersects)
				{
					info.IndexesForUpdateVertexes.push_back(info.CurrentIndex);
					info.IndexForUpdateNormalsOnIntersects.push_back(info.CurrentIndex);


					uint indexForAdd = info.CurrentIndex;
					if (isLeftLandBorderIntersects)
						++indexForAdd;

					if (isDownLandBorderIntersects)
						indexForAdd -= m_width + 1;

					downSide.push_back(indexForAdd);
				}
				else
				{
					downSide.push_back(info.CurrentIndex);
				}
			}
			else
			{
				downSide.push_back(info.CurrentIndex);
			}

			info.CurrentIndex += m_width + 1;
		}
		else {
			++info.CurrentIndex;
		}
	}

	// Find the most right index in the most down row
	info.CurrentIndex -= m_width;
	while (isInsideCircle(m_vertexes[info.CurrentIndex])) {
		++info.CurrentIndex;
	}

	// Prepare for fill DownRight Side
	info.PrevIndex = info.CurrentIndex - 1;

	// Fill DownRight Side
	while (info.CurrentIndex > info.StartIndex) {

		if (isInsideCircle(m_vertexes[info.CurrentIndex]) == false)
		{
			if (info.IsNeedToCheckBorderIntersects)
			{
				bool isRightLandBorderIntersects = info.CurrentIndex % info.RowLength == m_width;
				bool isDownLandBorderIntersects = info.CurrentIndex >= info.IndexBorder.DownLeft;

				if (isRightLandBorderIntersects || isDownLandBorderIntersects)
				{
					info.IndexesForUpdateVertexes.push_back(info.PrevIndex);
					info.IndexForUpdateNormalsOnIntersects.push_back(info.PrevIndex);


					uint indexForAdd = info.PrevIndex;
					if (isRightLandBorderIntersects)
						--indexForAdd;

					if (isDownLandBorderIntersects)
						indexForAdd -= m_width + 1;

					downSide.push_back(indexForAdd);
				}
				else
				{
					downSide.push_back(info.PrevIndex);
				}
			}
			else
			{
				downSide.push_back(info.PrevIndex);
			}

			info.PrevIndex = info.CurrentIndex - (m_width + 1);
			info.CurrentIndex -= m_width + 1;
		}
		else {
			info.PrevIndex = info.CurrentIndex;
			++info.CurrentIndex;
		}
	}

	return downSide;
}

std::vector<uint> Landscape::getCircleTopSide(InformationForUpdate info)
{
	if (info.CanUpdate == false)
		return std::vector<uint>();

	auto isInsideCircle = [squareMaxLength = qPow(info.ToolSize, 2), toolCenter = info.ToolCenter](VertexData &v) {
		return squareMaxLength <=
			qPow(v.position.x() - toolCenter.x, 2) +
			qPow(v.position.z() - toolCenter.y, 2);
	};

	std::vector<uint> topSide;

	// Prepare for fill TopLeft Side
	info.CurrentIndex = info.StartIndex - (m_width + 1);

	// Fill TopLeft Side
	while (info.CurrentIndex > info.IndexBorder.TopLeft) {

		if (isInsideCircle(m_vertexes[info.CurrentIndex]) == true) {
			if (info.IsNeedToCheckBorderIntersects)
			{
				bool isLeftLandBorderIntersects = info.CurrentIndex % info.RowLength == 0;
				bool isTopLandBorderIntersects = info.CurrentIndex < info.RowLength;

				if (isLeftLandBorderIntersects || isTopLandBorderIntersects)
				{
					info.IndexesForUpdateVertexes.push_back(info.CurrentIndex);
					info.IndexForUpdateNormalsOnIntersects.push_back(info.CurrentIndex);


					uint indexForAdd = info.CurrentIndex;
					if (isLeftLandBorderIntersects)
						++indexForAdd;

					if (isTopLandBorderIntersects)
						indexForAdd += m_width + 1;

					topSide.push_back(indexForAdd);
				}
				else
				{
					topSide.push_back(info.CurrentIndex);
				}
			}
			else
			{
				topSide.push_back(info.CurrentIndex);
			}

			info.CurrentIndex -= m_width + 1;
		}
		else {
			++info.CurrentIndex;
		}
	}

	// Find the most right index in the most down row
	info.CurrentIndex += info.RowLength;
	while (isInsideCircle(m_vertexes[info.CurrentIndex])) {
		++info.CurrentIndex;
	}

	// Prepare for fill DownRight Side
	info.PrevIndex = info.CurrentIndex - 1;

	// Fill TopRight Side
	while (info.CurrentIndex < info.StartIndex) {

		if (isInsideCircle(m_vertexes[info.CurrentIndex]) == false) {
			if (info.IsNeedToCheckBorderIntersects)
			{
				bool isRightLandBorderIntersects = info.CurrentIndex % info.RowLength == m_width;
				bool isTopLandBorderIntersects = info.CurrentIndex < info.RowLength;

				if (isRightLandBorderIntersects || isTopLandBorderIntersects)
				{
					info.IndexesForUpdateVertexes.push_back(info.PrevIndex);
					info.IndexForUpdateNormalsOnIntersects.push_back(info.PrevIndex);


					uint indexForAdd = info.PrevIndex;
					if (isRightLandBorderIntersects)
						--indexForAdd;

					if (isTopLandBorderIntersects)
						indexForAdd += m_width + 1;

					topSide.push_back(indexForAdd);
				}
				else
				{
					topSide.push_back(info.PrevIndex);
				}
			}
			else
			{
				topSide.push_back(info.PrevIndex);
			}

			info.CurrentIndex += m_width + 1;
		}
		else {
			info.PrevIndex = info.CurrentIndex;
			++info.CurrentIndex;
		}
	}

	return topSide;
}

void Landscape::updateVertexPositions(const std::vector<uint>& side, const std::vector<uint>& interiorSide, float toolStrength)
{
	uint l = 0, r = side.size() - 1;
	for (uint inner_r = interiorSide.size() - 1; l < interiorSide.size() / 2; ++l, --r, --inner_r)
	{
		// Fill inner circle
		for (uint index = interiorSide[l]; index <= interiorSide[inner_r]; ++index)
			m_vertexes[index].position.y += toolStrength;

		// Fill outer left part of the ring
		for (uint index = side[l]; index < interiorSide[l]; ++index)
			m_vertexes[index].position.y += toolStrength * getKoefDependsFromLen();

		// Fill outer right part of the ring
		for (uint index = interiorSide[inner_r] + 1; index <= side[r]; ++index)
			m_vertexes[index].position.y += toolStrength * getKoefDependsFromLen();
	}

	for (; l <= r; ++l, --r)
	{
		for (uint index = side[l]; index <= side[r]; ++index)
			m_vertexes[index].position.y += toolStrength;
	}
}

void Landscape::updateVertexesNormals(const std::vector<uint>& downSide, const std::vector<uint>& topSide)
{

	for (uint l = 0, r = downSide.size() - 1; l <= r; ++l, --r)
	{
		for (uint index = downSide[l]; index <= downSide[r]; ++index)
			m_vertexes[index].normal = calculateNormalByIndex(index);
	}
}



