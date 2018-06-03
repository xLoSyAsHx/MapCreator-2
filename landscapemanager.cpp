#include "landscapemanager.h"

#include "camera3d.h"

LandscapeManager::LandscapeManager() :
    m_landscape(nullptr)
{

}

LandscapeManager &LandscapeManager::Instance()
{
    static LandscapeManager l;
    return l;
}

void LandscapeManager::createLandscape(uint width, uint height, uint blockSize)
{
    if (!m_landscape.isNull())
    {
        m_landscape.clear();
    }


    m_landscape.reset(new Landscape(width, height, blockSize, [this]() {
        return this->GenerateObjectIdForLandscape();
    }));

}

void LandscapeManager::destroy()
{
    m_landscape.clear();
    m_landscape.reset(nullptr);
}

void LandscapeManager::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions, bool isSculptToolUse)
{


    if (m_landscape != nullptr)
    {
        m_landscape->draw(program, functions);
        m_landscape->setSculptToolUsage(isSculptToolUse);
    }
}

void LandscapeManager::objectPicking(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (m_landscape != nullptr)
        m_landscape->objectPicking(program, functions);
}

void LandscapeManager::updateLandscapeToolPosition(
        QVector3D mouseDir, QVector4D triangleId, QMatrix4x4 projectionMatrix)
{
    QMatrix4x4 model_matrix;
    model_matrix.setToIdentity();
    QMatrix4x4 mv_matrix = Camera3D::Instance().getViewMatrix() * model_matrix;
    QMatrix4x4 transformMatrix = projectionMatrix * mv_matrix;


    int intTriangleId = translateVec4IdToPosition(triangleId);
    --intTriangleId;

    if (intTriangleId < 0)
    {
        //qDebug() << "Cant found triangle";
        return;
    }

    //qDebug() << intTriangleId;

    Landscape::PrimitivePointPositions positions =
            m_landscape->getTrianglePointGlobalPositions(intTriangleId, projectionMatrix);

    //positions.pointA = transformMatrix * QVector4D(positions.pointA, 1).toVector3D();
    //positions.pointB = transformMatrix * QVector4D(positions.pointB, 1).toVector3D();
    //positions.pointC = transformMatrix * QVector4D(positions.pointC, 1).toVector3D();

    Plane3D plane(positions.pointA, positions.pointB, positions.pointC);

    float A = plane.getA();
    float B = plane.getB();
    float C = plane.getC();
    float D = plane.getD();



    QVector3D camPos = Camera3D::Instance().getPosition();
    QVector3D camPDir = Camera3D::Instance().getPosition() + mouseDir;
    //QVector4D xyz_0Vec3 = transformMatrix * QVector4D(camPos.x(), camPos.y(), camPos.z(), 1);
    //QVector4D xyz_1Vec3 = transformMatrix * QVector4D(camPDir.x(), camPDir.y(), camPDir.z(), 1);

    QVector4D xyz_0Vec3 = QVector4D(camPos.x(), camPos.y(), camPos.z(), 1);
    QVector4D xyz_1Vec3 = QVector4D(camPDir.x(), camPDir.y(), camPDir.z(), 1);

    float a = xyz_1Vec3.x() - xyz_0Vec3.x();
    float b = xyz_1Vec3.y() - xyz_0Vec3.y();
    float c = xyz_1Vec3.z() - xyz_0Vec3.z();

    float x0 = xyz_0Vec3.x();
    float y0 = xyz_0Vec3.y();
    float z0 = xyz_0Vec3.z();

    /* For test
    a = -1;
    b = -3;
    c = 3;

    x0 = -3;
    y0 = 0;
    z0 = -2;

    A = 1;
    B = -2;
    C = -1;
    D = 3;
    */


    float y = -(A * (b*x0 - a*y0) + C * (b*z0 - c*y0) + D * b) / (a*A + b*B + c*C);
    float x = (b*x0 + a*y - a*y0) / b;
    float z = (b*z0 + c*y - c*y0) / b;

    LandscapeSculptTool::Instance().setCenter(QVector2D(x, z));
}

void LandscapeManager::addTexture(aiTextureType type, QOpenGLTexture *image)
{
    m_landscape->addTexture(type, image);
}

void LandscapeManager::refreshByLandscapeTool()
{
    if (m_landscape != nullptr)
        m_landscape->refreshByLandscapeTool();
}

void LandscapeManager::updateLandscape()
{
    m_landscape.data()->refreshByLandscapeTool();

}

QVector4D LandscapeManager::GenerateObjectIdForLandscape()
{
    ++m_lastLandscapeId_d;

    if (m_lastLandscapeId_d > 255)
    {
        ++m_lastLandscapeId_c;
        m_lastLandscapeId_d = 1;
    }
    \

    if (m_lastLandscapeId_c > 255)
    {
        ++m_lastLandscapeId_b;
        m_lastLandscapeId_c = 1;
    }

    if (m_lastLandscapeId_b > 255)
    {
        ++m_lastLandscapeId_a;
        m_lastLandscapeId_b = 1;
    }

    QVector4D res =  QVector4D(
                float(m_lastLandscapeId_a) / 255.0f,
                float(m_lastLandscapeId_b) / 255.0f,
                float(m_lastLandscapeId_c) / 255.0f,
                float(m_lastLandscapeId_d) / 255.0f);
    return res;
}

int LandscapeManager::translateVec4IdToPosition(QVector4D vec)
{
    static int toInt = 255;
    static int cMultiplier = 255;
    static int bMultiplier = 255 * 255;
    static int aMultiplier = 255 * 255 * 255;

    int pos = 0;

    pos += (vec.w() * toInt - 1);
    pos += (vec.z() * toInt - 1) * cMultiplier;
    pos += (vec.y() * toInt - 1) * bMultiplier;
    pos += (vec.x() * toInt - 1) * aMultiplier;

    return pos;
}

