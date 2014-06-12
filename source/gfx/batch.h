#ifndef GFX_BATCH_H
#define GFX_BATCH_H

#include <x2d/math.h>
#include <x2d/util.h>

class Texture;
class Shader;
class VertexBufferObject;

// Vertex struct
struct XDAPI Vertex
{
	AS_DECL_VALUE

	Vertex() :
		position(0.0f),
		color(1.0f),
		texCoord(0.0f)
	{
	}
			
	// NOTE TO SELF: DO NOT ALTER ORDER OF DEFINITION
	Vector2 position;
	Vector4 color;
	Vector2 texCoord;
};

struct XDAPI VertexBuffer
{
	VertexBuffer() :
		vbo(0)
	{
	}

	vector<Vertex> vertices;
	vector<uint> indices;
	VertexBufferObject *vbo;
};

typedef map<Texture*, VertexBuffer> TextureVertexMap;

class XDAPI Batch
{
	friend class OpenGL;
public:
	AS_DECL_REF

	Batch();
	~Batch();
	
	// Batch projection matrix
	void setProjectionMatrix(const Matrix4 &projmat);
	Matrix4 getProjectionMatrix() const;

	// Add vertices and indices to the batch
	void addVertices(Vertex *vertices, int vertcount, uint *indices, int icount);
	void modifyVertex(int index, Vertex vertex);

	// Get vertex/vertex count
	Vertex getVertex(int index);
	int getVertexCount();

	// Get/set shader
	void setShader(Shader *shader);
	Shader *getShader() const;

	// Get/set texture
	void setTexture(Texture *texture);
	Texture *getTexture() const;

	// Render-to-texture
	Texture *renderToTexture();

	// Draw/upload/clear
	void draw();
	void clear();

	// Makes the batch static for increased performance (by using VBOs)
	// Note: While its called static, modifyVertex can still be accessed
	void makeStatic();
	bool isStatic() const;

private:
	void addVerticesAS(Array *vertices, Array *indices);

	TextureVertexMap m_buffers;
	Texture *m_texture;
	Shader *m_shader;
	bool m_static;
	Matrix4 m_projMatrix;

	static Batch *Factory() { return new Batch(); }
};

#endif // GFX_BATCH_H