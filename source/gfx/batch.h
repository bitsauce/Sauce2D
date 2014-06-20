#ifndef GFX_BATCH_H
#define GFX_BATCH_H

#include <x2d/math.h>
#include <x2d/util.h>
#include <x2d/scripts/array.h>

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
	VertexBuffer(int order) :
		order(order),
		vbo(0)
	{
	}
	
	bool operator<(const VertexBuffer& other) const
	{
		TUPLE_CMP(this->order, other.order)
		return false;
	}

	int order;
	vector<Vertex> vertices;
	vector<uint> indices;
	VertexBufferObject *vbo;
};

typedef map<Texture*, VertexBuffer*> TextureVertexMap;

class XDAPI Batch
{
	friend class OpenGL;
public:
	AS_DECL_REF

	Batch();
	virtual ~Batch();
	
	// Batch projection matrix
	void setProjectionMatrix(const Matrix4 &projmat);
	Matrix4 getProjectionMatrix() const;

	// Add vertices and indices to the batch
	void addVertices(Vertex *vertices, int vcount, uint *indices, int icount);
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

	// Draw/clear
	virtual void draw();
	virtual void clear();

	// Makes the batch static for increased performance (by using VBOs)
	// Note: While its called static, modifyVertex can still be accessed
	virtual void makeStatic();
	bool isStatic() const;

protected:

	TextureVertexMap m_buffers;
	Texture *m_texture;
	Shader *m_shader;
	bool m_static;
	Matrix4 m_projMatrix;
	//vector<Matrix4> m_matrixStack;
	int m_drawOrder;
	
	void addVerticesAS(Array *vertices, Array *indices);
	static Batch *Factory() { return new Batch(); }
};

class Sprite;

class SpriteBatch : public Batch
{
	AS_DECL_REF
public:

	~SpriteBatch();

	void add(Sprite *sprite);
	Sprite *get(int index);
	int getSize() const;

	void draw();
	void clear();
	void makeStatic();

private:
	vector<Sprite*> m_sprites;

	vector<Sprite*> m_returnedSprites;
	map<Sprite*, uint> m_offsets;
	
	static SpriteBatch *Factory() { return new SpriteBatch(); }
};

#endif // GFX_BATCH_H