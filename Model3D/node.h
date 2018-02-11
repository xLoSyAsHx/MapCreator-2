#ifndef NODE_H
#define NODE_H

#include <QSharedPointer>
#include <QMatrix4x4>
#include <QVector>

#include <QOpenGLShaderProgram>


class Node
{
public:

    void draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4 transformation);

private:
    QString m_name;

    QSharedPointer<Node> m_parent;
    QVector<QSharedPointer<Node>> m_childrens;

    QVector<unsigned int> m_meshIndexes;
    QMatrix4x4 m_transformation;

};

#endif // NODE_H
