#ifndef BONE_H
#define BONE_H


#include <QString>

class Bone
{
public:
    Bone();

    QString getName() const;

private:
    QString m_name;
};

#endif // BONE_H
