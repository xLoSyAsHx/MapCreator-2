#ifndef UTILS_H
#define UTILS_H

#include <QVector3D>

class Utils
{
    static int lastId_a;
    static int lastId_b;
    static int lastId_c;

public:

    static QVector3D GenerateObjectId();
};




#endif // UTILS_H
