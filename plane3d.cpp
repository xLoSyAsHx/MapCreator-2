#include "plane3d.h"



Plane3D::Plane3D(QVector3D a, QVector3D b, QVector3D c)
{
    QVector3D ab(b.x() - a.x(), b.y() - a.y(), b.z() - a.z());
    QVector3D ac(c.x() - a.x(), c.y() - a.y(), c.z() - a.z());


    QVector3D n = QVector3D::crossProduct(ab, ac);

    A = n.x();
    B = n.y();
    C = n.z();
    D = -A * b.x() - B * b.y() - C * b.z();
}

Plane3D::~Plane3D()
{

}

float Plane3D::getA() const
{
    return A;
}

float Plane3D::getB() const
{
    return B;
}

float Plane3D::getC() const
{
    return C;
}

float Plane3D::getD() const
{
    return D;
}

float Plane3D::getDistToPoint(QVector3D point) const
{
    return getDistToPoint(point.x(), point.y(), point.z());
}

float Plane3D::getDistToPoint(float x, float y, float z) const
{
    return float(
        qAbs(
            (A*x + B*y + C*z + D) /
            sqrt(A*A + B*B + C*C)
        )
    );
}

bool Plane3D::isInside(QVector3D vect)
{
    return A * vect.x() + B * vect.y() + C * vect.z() + D == 0;
}
