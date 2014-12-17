#ifndef X2D_BATCH_H
#define X2D_BATCH_H

#include "../engine.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include "texture.h"
#include "shader.h"

namespace xd
{

class FrameBufferObject;
class VertexBuffer;

/*********************************************************************
**	Batch															**
**********************************************************************/
class XDAPI Batch
{
	friend class Graphics;
public:
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

	// Primitive types
	enum PrimitiveType
	{
		PRIMITIVE_POINTS,
		PRIMITIVE_LINES,
		PRIMITIVE_TRIANGLES
	};
	
	// Batch projection matrix
	void setProjectionMatrix(const Matrix4 &projmat);
	Matrix4 getProjectionMatrix() const;

	void pushMatrix(const Matrix4 &mat);
	void popMatrix();

	// Blend func
	void setBlendFunc(const BlendFunc src, const BlendFunc dst);

	// Get/set shader
	void setShader(ShaderPtr shader);
	ShaderPtr getShader() const;

	// Get/set texture
	void setTexture(const Texture2DPtr &texture);
	Texture2DPtr getTexture() const;

	// Get/set primitive type
	void setPrimitive(PrimitiveType primitive);
	PrimitiveType getPrimitive() const;

	// Add vertices
	void addVertexBuffer(const shared_ptr<VertexBuffer> &buffer);
	void addVertices(Vertex *vertices, int vcount, uint *indices, int icount);

	// Render-to-texture
	void renderToTexture(Texture2DPtr texture);

	// Clear
	virtual void clear();
	
	struct State
	{
		State() :
			primitive(PRIMITIVE_TRIANGLES),
			texture(0),
			srcBlendFunc(BLEND_SRC_ALPHA),
			dstBlendFunc(BLEND_ONE_MINUS_SRC_ALPHA),
			shader(0),
			projMat()
		{
		}

		bool operator!=(const State &other) const
		{
			return this->primitive != other.primitive || this->projMat != other.projMat ||
				this->texture.get() != other.texture.get() || this->srcBlendFunc != other.srcBlendFunc ||
				this->dstBlendFunc != other.dstBlendFunc || this->shader != other.shader;
		}

		PrimitiveType primitive;
		Texture2DPtr texture;
		BlendFunc srcBlendFunc;
		BlendFunc dstBlendFunc;
		ShaderPtr shader;
		Matrix4 projMat;
	};

	struct VertexBufferState
	{
		State state;
		shared_ptr<VertexBuffer> buffer;
	};

protected:

	// State-vertex map
	vector<VertexBufferState> m_buffers;

	// Render target (for render-to-texture)
	RenderTarget2D *m_renderTarget;

	// Matrix stack
	stack<Matrix4> m_matrixStack;

	// Current batch state
	State m_state, m_prevState;
};

}

#endif // X2D_BATCH_H