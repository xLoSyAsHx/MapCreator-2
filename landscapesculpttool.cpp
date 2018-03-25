#include "landscapesculpttool.h"


bool LandscapeSculptTool::InsideCircleChecker::isInside(const QVector3D &pos) const
{
    return m_squaredToolRadius >=
        qPow(pos.x() - m_toolCenter.x(), 2) +
        qPow(pos.z() - m_toolCenter.y(), 2);
}



LandscapeSculptTool &LandscapeSculptTool::Instance()
{
    static LandscapeSculptTool landscapeSculptTool;
    return landscapeSculptTool;
}

QVector2D LandscapeSculptTool::getCenter() const
{
    return m_center;
}

float LandscapeSculptTool::getRadius() const
{
    return m_radius;
}

void LandscapeSculptTool::setCenter(QVector2D center)
{
    m_center = center;
}

void LandscapeSculptTool::setRadius(float size)
{
    m_radius = size;
}

float LandscapeSculptTool::getToolStrength() const
{
    return m_toolStrenght;
}

void LandscapeSculptTool::setToolStrength(float toolStrength)
{
    m_toolStrenght = toolStrength;
}

float LandscapeSculptTool::getBrushFalloff() const
{
    return m_brushFalloff;
}

void LandscapeSculptTool::setBryshFalloff(float brushFalloff)
{
    m_brushFalloff = brushFalloff;
}

float LandscapeSculptTool::getKoefDependsFromLen(const QVector3D & pos) const
{
    float falloffRadius = m_radius * m_brushFalloff;
    float lenFromPointToCenter = (QVector2D(pos.x(), pos.z()) - m_center).length();

    return 1.0f -  qMax(lenFromPointToCenter - falloffRadius, 0) / (m_radius - falloffRadius);
}


