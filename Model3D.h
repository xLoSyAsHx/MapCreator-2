#ifndef MODEL_3D_H
#define MODEL_3D_H

#include <QVector>
#include <QVector4D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


struct MaterialInfo
{
	QString Name;
	QVector3D Ambient;
	QVector3D Diffuse;
	QVector3D Specular;
	float Shininess;
};




struct LightInfo
{
	QVector4D Position;
	QVector3D Intensity;
};


struct Mesh
{
	QString name;
	unsigned int indexCount;
	unsigned int indexOffset;
	QSharedPointer<MaterialInfo> material;
};
	

struct Node
{
	Node() {}

	QString name;
	QMatrix4x4 transformation;
	QVector<QSharedPointer<Mesh> > meshes;
	QVector<Node> nodes;
};


class Model3D
{
public:
	
	friend class ModelLoader;

	Model3D();
	~Model3D();

    void drawNode(const QMatrix4x4& model, const Node *node, QMatrix4x4 parent);
    void draw(class QOpenGLShaderProgram *programm, class QOpenGLFunctions *functions);

private:

	/** Fields */

	QVector<float> m_vertices;
	QVector<float> m_normals;
	QVector<unsigned int> m_indices;

	QVector<QSharedPointer<MaterialInfo>> m_materials;
	QVector<QSharedPointer<Mesh>> m_meshes;
	QSharedPointer<Node> m_rootNode;


    QOpenGLShaderProgram m_pgm;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_normalBuffer;
    QOpenGLBuffer m_indexBuffer;

    class QOpenGLTexture *m_texture;


    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QMatrix4x4 m_projection, m_view;


    QString vertexShader =
            "#version 330 core\n"
            "\n"
            "layout(location = 0) in vec3 vertexPosition;\n"
            "layout(location = 1) in vec3 vertexNormal;\n"
            "\n"
            "out vec3 normal;\n"
            "out vec3 position;\n"
            "\n"
            "uniform mat4 MV;\n"
            "uniform mat3 N;\n"
            "uniform mat4 MVP;\n"
            " \n"
            "void main()\n"
            "{\n"
            "    normal = normalize( N * vertexNormal );\n"
            "    position = vec3( MV * vec4( vertexPosition, 1.0 ) );\n"
            "    gl_Position = MVP * vec4( vertexPosition, 1.0 );\n"
            "}\n"
            ;

    QString fragmentShader =
            "#version 330 core\n"
            "\n"
            "in vec3 normal;\n"
            "in vec3 position;\n"
            "\n"
            "layout (location = 0) out vec4 fragColor;\n"
            "\n"
            "struct Light\n"
            "{\n"
            "    vec4 position;\n"
            "    vec3 intensity;\n"
            "};\n"
            "uniform Light light;\n"
            "\n"
            "struct Material {\n"
            "    vec3 Ka;\n"
            "    vec3 Kd;\n"
            "    vec3 Ks;\n"
            "    float shininess;\n"
            "};\n"
            "uniform Material material;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec3 n = normalize( normal);\n"
            "    vec3 s = normalize( light.position.xyz - position);\n"
            "    vec3 v = normalize( -position.xyz);\n"
            "    vec3 h = normalize( v + s);\n"
            "    float sdn = dot( s, n);\n"
            "    vec3 ambient = material.Ka;\n"
            "    vec3 diffuse = material.Kd * max( sdn, 0.0);\n"
            "    vec3 specular = material.Ks * mix( 0.0, pow( dot(h, n), material.shininess), step( 0.0, sdn));\n"
            "    fragColor = vec4(light.intensity * (ambient + diffuse + specular), 1);\n"
            "}\n"
            ;

    /** Functions */
    void init();

};

#endif MODEL_3D_H
