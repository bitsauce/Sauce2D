#ifndef GFX_BATCH_H
#define GFX_BATCH_H

#include <x2d/math.h>
#include <x2d/util.h>
#include <x2d/scripts/array.h>

class Texture;
class Shader;
class VertexBufferObject;
class FrameBufferObject;

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

class XDAPI Batch
{
	friend class OpenGL;
public:
	AS_DECL_REF

	Batch();
	virtual ~Batch();
	
	// Blend func enum
	enum BlendFunc
	{
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_ONE_MINUS_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_ONE_MINUS_SRC_ALPHA,
		BLEND_DST_COLOR,
		BLEND_ONE_MINUS_DST_COLOR,
		BLEND_DST_ALPHA,
		BLEND_ONE_MINUS_DST_ALPHA,
		BLEND_SRC_ALPHA_SATURATE
	};
	
	// Batch projection matrix
	void setProjectionMatrix(const Matrix4 &projmat);
	Matrix4 getProjectionMatrix() const;

	// Blend func
	void setBlendFunc(const BlendFunc src, const BlendFunc dst);

	// Get/set shader
	void setShader(Shader *shader);
	Shader *getShader() const;

	// Get/set texture
	void setTexture(Texture *texture);
	Texture *getTexture() const;

	// Add vertices and indices to the batch
	void addVertices(Vertex *vertices, int vcount, uint *indices, int icount);
	void modifyVertex(int index, Vertex vertex);

	// Get vertex/vertex count
	Vertex getVertex(int index);
	int getVertexCount();

	// Render-to-texture
	void renderToTexture(Texture *texture);

	// Draw/clear
	virtual void draw();
	virtual void clear();

	// Makes the batch static for increased performance (by using VBOs)
	// Note: While its called static, modifyVertex can still be accessed
	virtual void makeStatic();
	bool isStatic() const;
	
	struct State
	{
		State() :
			drawOrder(0),
			texture(0),
			srcBlendFunc(BLEND_SRC_ALPHA),
			dstBlendFunc(BLEND_ONE_MINUS_SRC_ALPHA),
			shader(0)
		{
		}

		bool operator<(const State &other) const
		{
			TUPLE_CMP(this->drawOrder, other.drawOrder);
			TUPLE_CMP(this->texture, other.texture);
			TUPLE_CMP(this->srcBlendFunc, other.srcBlendFunc);
			TUPLE_CMP(this->dstBlendFunc, other.dstBlendFunc);
			TUPLE_CMP(this->shader, other.shader);
			return false;
		}

		int drawOrder;
		Texture *texture;
		BlendFunc srcBlendFunc;
		BlendFunc dstBlendFunc;
		Shader *shader;
	};

protected:

	// State-vertex map
	map<State, VertexBuffer*> m_buffers;

	// Texture drawing order map
	map<Texture*, int> m_drawOrderMap;

	// Frame buffer object (for render-to-texture)
	FrameBufferObject *m_fbo;

	// Static batch flag (vbo flag)
	bool m_static;

	// Projection matrix
	Matrix4 m_projMatrix;
	//vector<Matrix4> m_matrixStack;

	// Current batch state
	State m_state;
	
	void addVerticesAS(Array *vertices, Array *indices);
	static Batch *Factory() { return new Batch(); }
};

typedef map<Batch::State, VertexBuffer*> StateVertexMap;

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