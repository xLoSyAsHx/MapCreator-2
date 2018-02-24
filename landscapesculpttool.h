#ifndef LANDSCAPESCULPTTOOL_H
#define LANDSCAPESCULPTTOOL_H


#include <QVector2D>




class LandscapeSculptTool
{
public:

    enum class  Type {
        Circle
    };

    static LandscapeSculptTool& Instance();

    QVector2D getCenter() const;
    float getSize() const;

    void setCenter(QVector2D center);
    void setSize(float size);
    void setToolStrength(float toolStrength);
    void setBryshFalloff(float brushFalloff);


private:
    LandscapeSculptTool(){}
    LandscapeSculptTool(const LandscapeSculptTool&) = delete;
    LandscapeSculptTool& operator=(const LandscapeSculptTool&) = delete;


    QVector2D m_center;
    float m_size;
    float m_toolStrenght;
    float m_brushFalloff;
};

#endif // LANDSCAPESCULPTTOOL_H
