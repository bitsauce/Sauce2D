#ifndef X2D_BATCH_H
#define X2D_BATCH_H

#include "../engine.h"
#include "vertex.h"

class XTexture;
class XShader;
class XFrameBufferObject;
class XVertexBuffer;

/*********************************************************************
**	Batch															**
**********************************************************************/
class XDAPI XBatch
{
	friend class OpenGL;
public:
	AS_DECL_REF

	XBatch();
	virtual ~XBatch();
	
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

	// Blend func
	void setBlendFunc(const BlendFunc src, const BlendFunc dst);

	// Get/set shader
	void setShader(XShader *shader);
	XShader *getShader() const;

	// Get/set texture
	void setTexture(XTexture *texture);
	XTexture *getTexture() const;

	// Get/set primitive type
	void setPrimitive(PrimitiveType primitive);
	PrimitiveType getPrimitive() const;

	// Add vertices
	void setVertexBuffer(XVertexBuffer *buffer);
	void addVertices(XVertex *vertices, int vcount, uint *indices, int icount);

	// Render-to-texture
	void renderToTexture(XTexture *texture);

	// Draw/clear
	virtual void draw();
	virtual void clear();
	
	struct State
	{
		State() :
			drawOrder(0),
			primitive(PRIMITIVE_TRIANGLES),
			texture(0),
			srcBlendFunc(BLEND_SRC_ALPHA),
			dstBlendFunc(BLEND_ONE_MINUS_SRC_ALPHA),
			shader(0)
		{
		}

		bool operator<(const State &other) const
		{
			TUPLE_CMP(this->drawOrder, other.drawOrder);
			TUPLE_CMP(this->primitive, other.primitive);
			TUPLE_CMP(this->texture, other.texture);
			TUPLE_CMP(this->srcBlendFunc, other.srcBlendFunc);
			TUPLE_CMP(this->dstBlendFunc, other.dstBlendFunc);
			TUPLE_CMP(this->shader, other.shader);
			return false;
		}

		int drawOrder;
		PrimitiveType primitive;
		XTexture *texture;
		BlendFunc srcBlendFunc;
		BlendFunc dstBlendFunc;
		XShader *shader;
	};

protected:

	// State-vertex map
	map<State, XVertexBuffer*> m_buffers;

	// Texture drawing order map
	map<XTexture*, int> m_drawOrderMap;

	// Frame buffer object (for render-to-texture)
	XFrameBufferObject *m_fbo;

	// Projection matrix
	Matrix4 m_projMatrix;
	//vector<Matrix4> m_matrixStack;

	// Current batch state
	State m_state;
	
	void addVerticesAS(XScriptArray *vertices, XScriptArray *indices);
	static XBatch *Factory() { return new XBatch(); }
};

typedef map<XBatch::State, XVertexBuffer*> StateVertexMap;

#endif // X2D_BATCH_H