#include "utils.h"


QVector3D Utils::GenerateObjectId()
{
    ++lastId_c;

    if (lastId_c > 255)
    {
        ++lastId_b;
        lastId_c = 1;
    }

    if (lastId_b > 255)
    {
        ++lastId_a;
        lastId_c = 1;
    }

    return QVector3D(float(lastId_a) / 255.0f, float(lastId_b) / 255.0f, float(lastId_c) / 255.0f);
}

int Utils::lastId_a = 1;
int Utils::lastId_b = 1;
int Utils::lastId_c = 1;
