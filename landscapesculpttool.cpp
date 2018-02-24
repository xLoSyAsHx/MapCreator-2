#include "landscapesculpttool.h"


LandscapeSculptTool &LandscapeSculptTool::Instance()
{
    static LandscapeSculptTool landscapeSculptTool;
    return landscapeSculptTool;
}

QVector2D LandscapeSculptTool::getCenter() const
{
    return m_center;
}

float LandscapeSculptTool::getSize() const
{
    return m_size;
}

void LandscapeSculptTool::setCenter(QVector2D center)
{
    m_center = center;
}

void LandscapeSculptTool::setSize(float size)
{
    m_size = size;
}

void LandscapeSculptTool::setToolStrength(float toolStrength)
{
    m_toolStrenght = toolStrength;
}

void LandscapeSculptTool::setBryshFalloff(float brushFalloff)
{
    m_brushFalloff = brushFalloff;
}
