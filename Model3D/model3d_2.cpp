#include "model3d_2.h"

Model3D_2::Model3D_2(QString path)
{

}

void Model3D_2::draw(QOpenGLShaderProgram *shaderProgram)
{
    m_rootNode.data()->draw(shaderProgram, m_globalTransformation);
}