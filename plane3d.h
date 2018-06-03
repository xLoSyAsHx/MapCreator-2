#ifndef PLANE3D_H
#define PLANE3D_H

#include <QVector3D>

class Plane3D
{

public:
    Plane3D(QVector3D a, QVector3D b, QVector3D c);
    ~Plane3D();

    float getA() const;
    float getB() const;
    float getC() const;
    float getD() const;

    float getDistToPoint(float x, float y, float z) const;
    float getDistToPoint(QVector3D point) const;
    bool isInside(QVector3D vect);

private:
    float A, B, C, D;
};

#endif // PLANE3D_H
