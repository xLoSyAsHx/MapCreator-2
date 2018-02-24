#include "landscape.h"

Landscape::Landscape(uint width, uint height, uint blockSize) :
    m_width(width), m_length(height), m_blockSize(blockSize),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{

    fillIndexes(width, height);
    fillVertexes(width, height, blockSize);
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
    for (uint i = 0; i < m_length + 1; ++i) {
        for (uint j = 0; i < m_width + 1; ++j) {
            QVector3D normal;

            QVector<uint> faceIndexes = {
                i * (m_width * 2) + j * 2 - 1, // Top left
                i * (m_width * 2) + j * 2, // Top middle
                i * (m_width * 2) + j * 2 + 1, // Top right
                (i + 1) * (m_width * 2) + j * 2 - 2, // Down left
                (i + 1) * (m_width * 2) + j * 2 - 1, // Down middle
                (i + 1) * (m_width * 2) + j * 2 // Down right
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

    return QVector3D::crossProduct(
                m_vertexes[ind1].position - m_vertexes[ind2].position,
                m_vertexes[ind3].position - m_vertexes[ind2].position);
}

Border2D Landscape::refreshByLandscapeTool()
{
     return getSmallestBorderForTool(LandscapeSculptTool::Instance().getCenter(),
                                           LandscapeSculptTool::Instance().getSize());
}

Border2D Landscape::refreshByLandscapeTool(QVector2D center, float size)
{
    return getSmallestBorderForTool(center, size);
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


bool Landscape::SmallestBorderForToolCalculator::isIntersect(const Border2D &border)
{
    bool isBetweenTopAndDown = m_center.y() >= border.TopLeft.y() && m_center.y() <= border.DownLeft.y();
    bool isBetweenLeftAndRight = m_center.x() >= border.DownLeft.x() && m_center.x() <= border.DownRight.x();

    // If circle center inside border
    if (isBetweenTopAndDown && isBetweenLeftAndRight)
        return true;

    // If circle intersects border's corners
    if ((border.TopLeft - m_center).length() - m_size <= 0 ||
            (border.TopRight - m_center).length() - m_size <= 0 ||
            (border.DownLeft - m_center).length() - m_size <= 0 ||
            (border.DownRight - m_center).length() - m_size <= 0)
        return true;



    // If circle intersects border's sides
    if (isBetweenTopAndDown && m_center.x() <= border.TopLeft.x() && border.TopLeft.x() - m_center.x() <= m_size || // Left
            isBetweenTopAndDown && m_center.x() >= border.TopRight.x() && m_center.x() - border.TopRight.x() <= m_size || // Right
            isBetweenLeftAndRight && m_center.y() <= border.TopLeft.y() && border.TopLeft.y() - m_center.y() <= m_size || // Top
            isBetweenLeftAndRight && m_center.y() >= border.DownLeft.y() && m_center.y() - border.DownLeft.y() <= m_size) // Down
        return true;

    return false;
}













