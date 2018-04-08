#include "landscape.h"

Landscape::Landscape(uint width, uint height, uint blockSize) :
    m_width(width), m_length(height), m_blockSize(blockSize),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{

    fillIndexes(width, height);
    fillVertexes(width, height, blockSize);

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indexes.constData(), m_indexes.size() * sizeof(uint));
    m_indexBuffer.release();

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertexes.constData(), m_vertexes.size() * sizeof(VertexData));
    m_vertexBuffer.release();
}

Landscape::~Landscape()
{
    int asd;
}

void Landscape::rotate(const QQuaternion &rotation)
{

}

void Landscape::translate(const QVector3D &translation)
{

}

void Landscape::scale(const float scaleKoef)
{

}

void Landscape::setGlobalTransform(const QMatrix4x4 &matrix)
{

}

void Landscape::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    shaderProgram->setUniformValue("u_modelMatrix", modelMatrix);

    // Bind buffers
    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    int offset = 0;

    // Set varying attribute "a_position"
    int location = shaderProgram->attributeLocation("a_position");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));


    offset += sizeof(QVector3D);
    // Set varying attribute "a_textCoord"
    location = shaderProgram->attributeLocation("a_textCoord");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 2, sizeof(VertexData));


    offset += sizeof(QVector2D);
    // Set varying attribute "a_normal"
    location = shaderProgram->attributeLocation("a_normal");
    shaderProgram->enableAttributeArray(location);
    shaderProgram->setAttributeBuffer(location, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Draw
    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);

    // Release
    m_indexBuffer.release();
    m_vertexBuffer.release();
}

void Landscape::calculateNormals()
{
    for (int i = 0; i < m_length + 1; ++i) {
        for (int j = 0; i < m_width + 1; ++j) {
            QVector3D normal;

            QVector<int> faceIndexes = {
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

            normal.normalize();
            m_vertexes[i * (m_width + 1) + j].normal = normal;
        }
    }
}

void Landscape::resetBuffers()
{
    if (m_indexBuffer.isCreated() != true) {
        m_indexBuffer.create();
    }

    m_indexBuffer.bind();
    if (m_indexBuffer.size() == 0) {
        m_indexBuffer.allocate(m_indexes.size() * sizeof(uint));
    }
    else {
        //m_indexBuffer.write();
    }

    m_indexBuffer.release();

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

    QVector3D halfLensOfLandscape(width * blockSize / 2, 0, height * blockSize / 2);

    for (uint i = 0; i < height + 1; ++i) {
        for (uint j = 0; j < width + 1; ++j) {
            m_vertexes[i * (width + 1) + j].position = QVector3D(blockSize * j, 0, blockSize * i) - halfLensOfLandscape;
            m_vertexes[i * (width + 1) + j].normal = QVector3D(0, 1, 0);
        }
    }
}

bool Landscape::isCircleIntersectsLandscape(QVector2D circleCenter, float circleSize)
{
    Border2D border(QVector2D(-m_width / 2, -m_length / 2), // TopLeft
                             QVector2D(m_width / 2 + (m_width % 2), -m_length / 2), // TopRight
                             QVector2D(-m_width / 2, m_length / 2 + (m_length % 2)), // DownLeft
                             QVector2D(m_width / 2 + (m_width % 2), m_length / 2 + (m_length % 2))); // DownRight

    border *= m_blockSize;

    bool isBetweenTopAndDown = circleCenter.y() >= border.TopLeft.y() && circleCenter.y() <= border.DownLeft.y();
    bool isBetweenLeftAndRight = circleCenter.x() >= border.DownLeft.x() && circleCenter.x() <= border.DownRight.x();

    // If circle center inside border
    if (isBetweenTopAndDown && isBetweenLeftAndRight)
        return true;

    // If circle intersects border's corners
    if ((border.TopLeft - circleCenter).length() - circleSize <= 0 ||
            (border.TopRight - circleCenter).length() - circleSize <= 0 ||
            (border.DownLeft - circleCenter).length() - circleSize <= 0 ||
            (border.DownRight - circleCenter).length() - circleSize <= 0)
        return true;



    // If circle intersects border's sides
    if (isBetweenTopAndDown && circleCenter.x() <= border.TopLeft.x() && border.TopLeft.x() - circleCenter.x() <= circleSize || // Left
            isBetweenTopAndDown && circleCenter.x() >= border.TopRight.x() && circleCenter.x() - border.TopRight.x() <= circleSize || // Right
            isBetweenLeftAndRight && circleCenter.y() <= border.TopLeft.y() && border.TopLeft.y() - circleCenter.y() <= circleSize || // Top
            isBetweenLeftAndRight && circleCenter.y() >= border.DownLeft.y() && circleCenter.y() - border.DownLeft.y() <= circleSize) // Down
        return true;

    return false;
}

int Landscape::getShiftedIndexForPointOnBorderIntersects(int index)
{

    bool isLeftLandBorderIntersects = index % (m_width + 1) == 0;
    bool isTopLandBorderIntersects = index < m_width + 1;
    bool isRightLandBorderIntersects = index % (m_width + 1) == m_width;
    bool isDownLandBorderIntersects = index >= m_length * (m_width + 1);

    if (isDownLandBorderIntersects || isTopLandBorderIntersects)
        return -2;

    if (isRightLandBorderIntersects  || isDownLandBorderIntersects
            || isLeftLandBorderIntersects || isTopLandBorderIntersects)
    {
        uint indexForAdd = index;
        if (isRightLandBorderIntersects)
            --indexForAdd;


        //if (isDownLandBorderIntersects)
            //indexForAdd -= m_width + 1;


        if (isLeftLandBorderIntersects)
            ++indexForAdd;


        //if (isTopLandBorderIntersects)
            //indexForAdd += m_width + 1;


        return indexForAdd;
    }
   // else

    else
        return -1;
}

QVector3D Landscape::getNormalByFaceIndex(uint index) const
{
    uint ind1 = m_indexes[3 * index];
    uint ind2 = m_indexes[3 * index + 1];
    uint ind3 = m_indexes[3 * index + 2];

    return QVector3D::crossProduct(
                m_vertexes[ind1].position - m_vertexes[ind2].position,
                m_vertexes[ind3].position - m_vertexes[ind2].position);
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

IndexBorder2D Landscape::getIndexBorderForToolBySize(QVector2D toolCenter, float circleRadius)
{
    IndexBorder2D border;

    if (isCircleIntersectsLandscape(toolCenter, circleRadius))
    {
        // Определить примерный прямоугольник, в котором находится центр
        int leftX = qMax(int(toolCenter.x() - circleRadius), -m_width / 2); // 41.2 -> 41
        int rightX = qMin(int(toolCenter.x() + circleRadius), m_width / 2);
        int topZ = qMax(int(toolCenter.y() - circleRadius), -m_length / 2);
        int downZ = qMin(int(toolCenter.y() + circleRadius), m_length / 2 + m_length % 2);
        //int numIndexesInRow = rightX - leftX + 1;
        //int numIndexesInColumn = downZ - topZ + 1;

        // Определить верхнюю левую и нижнюю левую точки для перерасчёта координат и нормалей
        border.TopLeft = getIndexByShiftFromCenter(leftX, topZ);
        border.DownLeft = getIndexByShiftFromCenter(leftX, downZ);
        border.TopRight = getIndexByShiftFromCenter(rightX, topZ);
        border.DownRight = getIndexByShiftFromCenter(rightX, downZ);
        border.IsValid = true;
    }
    else
        border.IsValid = false;

    return border;
}

QPair<int, int> Landscape::getStartAndXIndexes(const IndexBorder2D &border, const QVector2D &toolCenter, float circleRadius) const
{
    uint numOfRows = (border.DownLeft - border.TopLeft) / (m_width + 1) + 1;

        int startIndex = -1;

        //check (x_index + 1_row), (x_index), (x_index - 1_row) and (x_index - 2_row)
        uint x_index = border.TopLeft + (numOfRows / 2) * (m_width + 1);

        LandscapeSculptTool::InsideCircleChecker checker(circleRadius, toolCenter);
        for (int i = -1; i < 3; ++i) {
            uint index = x_index - i * (m_width + 1);

            if (index >= border.TopLeft && index <= border.DownLeft + m_width + 1 &&
                    checker.isInside(m_vertexes[index].position))
            {
                startIndex = index;
                break;
            }
        }

        return qMakePair(startIndex, x_index);
}

uint Landscape::getIndexByShiftFromCenter(int x, int y) const
{
    uint columnShift = (m_width / 2) + x;
    uint rowShift = (m_length / 2) + y;

    return rowShift * (m_width + 1) + columnShift;
}

void Landscape::refreshByLandscapeTool()
{
    LandscapeSculptTool& tool = LandscapeSculptTool::Instance();

    if (isCircleIntersectsLandscape(tool.getCenter(), tool.getRadius()) == false)
        return;

    QVector<uint> indexesForUpdateNormals;

    InformationForUpdate infoForUpdate(tool.getCenter(), tool.getRadius(), m_width + 1);

    prepareInfoForUpdateOuterRing(infoForUpdate);
    QVector<uint> downSide = getCircleDownSide(infoForUpdate);
    QVector<uint> topSide = getCircleTopSide(infoForUpdate);


    if (isCircleIntersectsLandscape(tool.getCenter(), tool.getRadius() * tool.getBrushFalloff()) == false)
        infoForUpdate.CanUpdate = false;
    else
        prepareInfoForUpdateInnerCircle(infoForUpdate);

    QVector<uint> downInteriorSide = getCircleDownSide(infoForUpdate);
    QVector<uint> topInteriorSide = getCircleTopSide(infoForUpdate);

    // startIndex должен быть на той же строке на которой был при расчёте внешнего кольца
    //fillPerimeter(downInteriorSide, topInteriorSide, indexesForUpdateVertexes, indexForUpdateNormalsOnIntersects,
    //	toolCenter, size * toolFalloff, false);

    // Update Down side
    updateVertexPositions(downSide, downInteriorSide, tool.getToolStrength());

    // Update Top side
    updateVertexPositions(topSide, topInteriorSide, tool.getToolStrength());

    infoForUpdate.ToolRadius = tool.getRadius();

    // Find and update vertex positions on intersects
    //findAndUpdateVertexesOnBorderIntersects(infoForUpdate);


    prepareInfoForUpdateOuterRing(infoForUpdate);

    // Обновлять нормали надо только когда перестаёт быть активным LandscapeSculptTool

    qDebug() << "Before rewrite buffer";

    // Определить с какого места и сколько VertexDatas надо обновить в вершинном буфере
    int offset = infoForUpdate.IndexBorder.TopLeft * sizeof(VertexData);
    int count = (infoForUpdate.IndexBorder.DownRight - infoForUpdate.IndexBorder.TopLeft + 1) * sizeof(VertexData);

    m_vertexBuffer.bind();
    m_vertexBuffer.write(offset, m_vertexes.data() + infoForUpdate.IndexBorder.TopLeft, count);
    m_vertexBuffer.release();



    // Обновить данные в вершинном буфере



    //return getSmallestBorderForTool(center, size);
}


void Landscape::prepareInfoForUpdateOuterRing(Landscape::InformationForUpdate &info)
{
    info.IndexBorder = getIndexBorderForToolBySize(info.ToolCenter, info.ToolRadius);
    info.RowLength = m_width + 1;
    info.LandscapeDownLeftIndex = m_vertexes.size() - info.RowLength;

    QPair<int, int> startAndX_indexes = getStartAndXIndexes(info.IndexBorder, info.ToolCenter, info.ToolRadius);
    info.StartIndex = startAndX_indexes.first;

    if (info.StartIndex == -1)
        info.StartIndex = info.CurrentIndex = info.PrevIndex = startAndX_indexes.second + 1;
    else
        info.CurrentIndex = info.PrevIndex = info.StartIndex;

    info.CanUpdate = true;
}

void Landscape::prepareInfoForUpdateInnerCircle(Landscape::InformationForUpdate &info)
{
    info.ToolRadius = LandscapeSculptTool::Instance().getRadius() * LandscapeSculptTool::Instance().getBrushFalloff();
    info.IndexBorder = getIndexBorderForToolBySize(info.ToolCenter, info.ToolRadius);
    info.IsNeedToCheckBorderIntersects = false;

    LandscapeSculptTool::InsideCircleChecker checker(info.ToolRadius, info.ToolCenter);

    for (int i = 0; ; ++i)
    {
        int index = info.StartIndex + i;

        if (checker.isInside(m_vertexes[index].position))
        {
            info.StartIndex = info.CurrentIndex = info.PrevIndex = index;
            break;
        }
    }

    /*
    QPair<int, int> startAndX_indexes = getStartAndXIndexes(info.IndexBorder, info.ToolCenter, info.ToolRadius);
    info.StartIndex = startAndX_indexes.first;

    if (info.StartIndex == -1)
        info.StartIndex = info.CurrentIndex = info.PrevIndex = startAndX_indexes.second + 1;
    else
        info.CurrentIndex = info.PrevIndex = info.StartIndex;
        */

}

QVector<uint> Landscape::getCircleDownSide(Landscape::InformationForUpdate info)
{
    if (info.CanUpdate == false)
            return QVector<uint>();

    LandscapeSculptTool::InsideCircleChecker checker(info.ToolRadius, info.ToolCenter);
    QVector<uint> downSide;

    // Fill DownLeft Side
    while (info.CurrentIndex < info.IndexBorder.DownLeft + info.RowLength) {

        if (checker.isInside(m_vertexes[info.CurrentIndex].position) == true)
        {
            downSide.push_back(info.CurrentIndex);
            info.CurrentIndex += info.RowLength;
        }
        else
        {
            ++info.CurrentIndex;
        }
    }

    // Find the most right index in the most down row
    //info.CurrentIndex -= info.RowLength;
    if (downSide.isEmpty() == true)
        return downSide;

    info.CurrentIndex = downSide.last();
    while (checker.isInside(m_vertexes[info.CurrentIndex].position)) {
        ++info.CurrentIndex;
    }

    // Prepare for fill DownRight Side
    info.PrevIndex = info.CurrentIndex - 1;

    // Fill DownRight Side
    while (info.CurrentIndex > info.StartIndex)
    {
        bool isRowChanged = false;

        // Because if row changed that we can add PrevIndex to the vector.
        // It's the defence from case when row changed but CurrentIndex is still inside circle
        if (checker.isInside(m_vertexes[info.CurrentIndex].position) == false || isRowChanged == true)
        {
            downSide.push_back(info.PrevIndex);

            info.PrevIndex = info.CurrentIndex - info.RowLength - 1;
            info.CurrentIndex -= info.RowLength + 1;
        }
        else
        {
            info.PrevIndex = info.CurrentIndex;
            ++info.CurrentIndex;

            // Flag for protect from infinite cycle. For more info check the begin of the cycle
            if (info.CurrentIndex % info.RowLength == 0)
                isRowChanged = true;
        }
    }

    return downSide;
}

QVector<uint> Landscape::getCircleTopSide(Landscape::InformationForUpdate info)
{
    if (info.CanUpdate == false)
            return QVector<uint>();

    LandscapeSculptTool::InsideCircleChecker checker(info.ToolRadius, info.ToolCenter);
    QVector<uint> topSide;

    // Prepare for fill TopLeft Side
    info.CurrentIndex = info.StartIndex - (m_width + 1);

    // Fill TopLeft Side
    while (info.CurrentIndex >= info.IndexBorder.TopLeft) {

        if (checker.isInside(m_vertexes[info.CurrentIndex].position) == true)
        {
            topSide.push_back(info.CurrentIndex);
            info.CurrentIndex -= info.RowLength;
        }
        else {
            ++info.CurrentIndex;
        }
    }

    // Find the most right index in the most down row
    //info.CurrentIndex += info.RowLength;
    if (topSide.isEmpty() == true)
        return topSide;

    info.CurrentIndex = topSide.last();
    while (checker.isInside(m_vertexes[info.CurrentIndex].position)) {
        ++info.CurrentIndex;
    }

    // Prepare for fill DownRight Side
    info.PrevIndex = info.CurrentIndex - 1;

    // Fill TopRight Side
    while (info.CurrentIndex < info.StartIndex) {
        if (checker.isInside(m_vertexes[info.CurrentIndex].position) == false)
        {
            topSide.push_back(info.PrevIndex);
            info.CurrentIndex += info.RowLength;
            info.PrevIndex = info.CurrentIndex - 1;
        }
        else
        {
            info.PrevIndex = info.CurrentIndex;
            ++info.CurrentIndex;
        }
    }

    return topSide;
}

void Landscape::updateVertexPositions(const QVector<uint> &side, const QVector<uint> &interiorSide, float toolStrength)
{
    LandscapeSculptTool & tool = LandscapeSculptTool::Instance();

    uint l = 0, r = side.size() - 1;
    for (uint inner_r = interiorSide.size() - 1; l < interiorSide.size() / 2; ++l, --r, --inner_r)
    {
        // Fill inner circle
        for (uint index = interiorSide[l]; index <= interiorSide[inner_r]; ++index) {
            QVector3D & pos = m_vertexes[index].position;
            pos.setY(pos.y() + toolStrength);
        }

        // Fill outer left part of the ring
        for (uint index = side[l]; index < interiorSide[l]; ++index) {
            QVector3D & pos = m_vertexes[index].position;
            pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
        }

        // Fill outer right part of the ring
        for (uint index = interiorSide[inner_r] + 1; index <= side[r]; ++index) {
            QVector3D & pos = m_vertexes[index].position;
            pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
        }
    }

    for (; l <= r; ++l, --r)
    {
        for (uint index = side[l]; index <= side[r]; ++index) {
            QVector3D & pos = m_vertexes[index].position;
            pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
        }
    }
}

void Landscape::updateVertexesNormals(const QVector<uint> &side)
{
    for (uint l = 0, r = side.size() - 1; l <= r; ++l, --r)
    {
        for (uint index = side[l]; index <= side[r]; ++index)
            m_vertexes[index].normal = calculateNormalByIndex(index);
    }
}

void Landscape::findAndUpdateVertexesOnBorderIntersects(InformationForUpdate const &info)
{   
    LandscapeSculptTool & tool = LandscapeSculptTool::Instance();
    float toolStrength = tool.getToolStrength();

    if (info.IndexBorder.DownLeft % info.RowLength == 0) // Is circle intersects by left side
    {
        int low = info.IndexBorder.TopLeft;
        int high = info.IndexBorder.DownLeft;
        int step = info.RowLength;

        QPair<int, int> low_high = getBordersOnIntersects(info, low, high, step);

        if (low_high.first == -1 && low_high.second != -1)
            low_high.first = low_high.second;
        else if (low_high.first != -1 && low_high.second == -1)
            low_high.second = low_high.first;


        if (low_high.first != -1 && low_high.second != -1)
        {
            for (int index = low_high.first; index <= low_high.second; index += step) {
                QVector3D & pos = m_vertexes[index].position;
                pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
            }
        }
    }

    if (info.IndexBorder.DownLeft >= m_length * info.RowLength) // Is circle intersects by down side
    {
        int low = info.IndexBorder.DownLeft;
        int high = info.IndexBorder.DownLeft + m_width;
        int step = 1;

        QPair<int, int> low_high = getBordersOnIntersects(info, low, high, step);

        if (low_high.first == -1 && low_high.second != -1)
            low_high.first = low_high.second;
        else if (low_high.first != -1 && low_high.second == -1)
            low_high.second = low_high.first;


        if (low_high.first != -1 && low_high.second != -1)
        {
            for (int index = low_high.first; index <= low_high.second; index += step) {
                QVector3D & pos = m_vertexes[index].position;
                pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
            }
        }
    }

    if (info.IndexBorder.TopRight % info.RowLength == m_width) // Is circle intersects by right side
    {
        int low = info.IndexBorder.TopRight;
        int high = info.IndexBorder.DownRight;
        int step = info.RowLength;

        QPair<int, int> low_high = getBordersOnIntersects(info, low, high, step);

        if (low_high.first == -1 && low_high.second != -1)
            low_high.first = low_high.second;
        else if (low_high.first != -1 && low_high.second == -1)
            low_high.second = low_high.first;


        if (low_high.first != -1 && low_high.second != -1)
        {
            for (int index = low_high.first; index <= low_high.second; index += step) {
                QVector3D & pos = m_vertexes[index].position;
                pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
            }
        }
    }

    if (info.IndexBorder.TopLeft < info.RowLength) // Is circle intersects by top side
    {
        int low = info.IndexBorder.TopLeft;
        int high = info.IndexBorder.TopLeft + m_width;
        int step = 1;

        QPair<int, int> low_high = getBordersOnIntersects(info, low, high, step);

        if (low_high.first == -1 && low_high.second != -1)
            low_high.first = low_high.second;
        else if (low_high.first != -1 && low_high.second == -1)
            low_high.second = low_high.first;


        if (low_high.first != -1 && low_high.second != -1)
        {
            for (int index = low_high.first; index <= low_high.second; index += step) {
                QVector3D & pos = m_vertexes[index].position;
                pos.setY(pos.y() + toolStrength * tool.getKoefDependsFromLen(pos));
            }
        }
    }

}

QPair<int, int> Landscape::getBordersOnIntersects(const InformationForUpdate & info,
                                                  int low, int high, int step)
{
    LandscapeSculptTool::InsideCircleChecker checker(info.ToolRadius, info.ToolCenter);
    int minIntersect = low, maxIntersect = high;
    QPair<int, int> res{-1, -1};


    // Min side
    while (true)
    {
        if (maxIntersect - minIntersect == step)
            break;

        int center = ((low + high) / 2 / step) * step;
        bool isInside = checker.isInside(m_vertexes[center].position);

        if (isInside)
            maxIntersect = center;
        else
            minIntersect = center;
    }

    if (checker.isInside(m_vertexes[minIntersect].position))
        res.first = minIntersect;
    else if (checker.isInside(m_vertexes[maxIntersect].position))
        res.first = maxIntersect;

    minIntersect = low;
    maxIntersect = high;

    // Max side
    while (true)
    {
        if (maxIntersect - minIntersect == step)
            break;

        int center = ((low + high) / 2 / step) * step;
        bool isInside = checker.isInside(m_vertexes[center].position);

        if (isInside)
            minIntersect = center;
        else
            maxIntersect = center;
    }

    if (checker.isInside(m_vertexes[maxIntersect].position))
        res.second = maxIntersect;
    else if (checker.isInside(m_vertexes[minIntersect].position))
        res.second = minIntersect;

    return res;
}















