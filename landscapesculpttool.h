#ifndef LANDSCAPESCULPTTOOL_H
#define LANDSCAPESCULPTTOOL_H

#include <QtMath>
#include <QVector2D>

#include "Model3D/vertexdata.h"


class LandscapeSculptTool
{
public:

    enum class  Type {
        Circle
    };

    class InsideCircleChecker {
    public:
        InsideCircleChecker(float toolRadius, QVector2D toolCenter) :
            m_squaredToolRadius(qPow(toolRadius, 2)), m_toolCenter(toolCenter)
        {
        }

        bool isInside(const QVector3D &v) const;

    private:
        float m_squaredToolRadius;
        QVector2D m_toolCenter;
    };

    static LandscapeSculptTool& Instance();

    QVector2D getCenter() const;
    void setCenter(QVector2D center);

    float getRadius() const;
    void setRadius(float size);

    float getToolStrength() const;
    void setToolStrength(float toolStrength);

    float getBrushFalloff() const;
    void setBryshFalloff(float brushFalloff);

    float getKoefDependsFromLen(const QVector3D & pos) const;

private:
    LandscapeSculptTool(){}
    LandscapeSculptTool(const LandscapeSculptTool&) = delete;
    LandscapeSculptTool& operator=(const LandscapeSculptTool&) = delete;


    QVector2D m_center;
    float m_radius; // Radius
    float m_toolStrenght;
    float m_brushFalloff;
};

#endif // LANDSCAPESCULPTTOOL_H
