//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

AS_REG_REF(XBatch, "Batch")

int XBatch::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterEnum("BlendFunc"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ZERO", BLEND_ZERO); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ONE", BLEND_ONE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_SRC_COLOR", BLEND_SRC_COLOR); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ONE_MINUS_SRC_COLOR", BLEND_ONE_MINUS_SRC_COLOR); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_SRC_ALPHA", BLEND_SRC_ALPHA); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ONE_MINUS_SRC_ALPHA", BLEND_ONE_MINUS_SRC_ALPHA); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_DST_COLOR", BLEND_DST_COLOR); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ONE_MINUS_DST_COLOR", BLEND_ONE_MINUS_DST_COLOR); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_DST_ALPHA", BLEND_DST_ALPHA); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_ONE_MINUS_DST_ALPHA", BLEND_ONE_MINUS_DST_ALPHA); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("BlendFunc", "BLEND_SRC_ALPHA_SATURATE", BLEND_SRC_ALPHA_SATURATE); AS_ASSERT

	r = scriptEngine->RegisterObjectBehaviour("Batch", asBEHAVE_FACTORY, "Batch @f()", asFUNCTIONPR(Factory, (), XBatch*), asCALL_CDECL); AS_ASSERT
		
	// Getters/setters
	r = scriptEngine->RegisterObjectMethod("Batch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(XBatch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setShader(Shader @shader)", asMETHOD(XBatch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setTexture(Texture @texture)", asMETHOD(XBatch, setTexture), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setBlendFunc(const BlendFunc, const BlendFunc)", asMETHOD(XBatch, setBlendFunc), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Matrix4 getProjectionMatrix() const", asMETHOD(XBatch, getProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Shader @getShader() const", asMETHOD(XBatch, getShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Texture @getTexture() const", asMETHOD(XBatch, getTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Batch", "void addVertices(array<Vertex> @vertices, array<uint> @indices)", asMETHOD(XBatch, addVerticesAS), asCALL_THISCALL); AS_ASSERT

	// Misc
	r = scriptEngine->RegisterObjectMethod("Batch", "void draw()", asMETHOD(XBatch, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void clear()", asMETHOD(XBatch, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void renderToTexture(Texture@)", asMETHOD(XBatch, renderToTexture), asCALL_THISCALL); AS_ASSERT

	return r;
}

XBatch::XBatch() :
	m_fbo(0)
{
}

XBatch::~XBatch()
{
	delete m_fbo;
	clear();
}

void XBatch::setBlendFunc(const BlendFunc src, const BlendFunc dst)
{
	m_state.srcBlendFunc = src;
	m_state.dstBlendFunc = dst;
}

void XBatch::setProjectionMatrix(const Matrix4 &projmat)
{
	m_projMatrix = projmat;
}

Matrix4 XBatch::getProjectionMatrix() const
{
	return m_projMatrix;
}

void XBatch::setShader(XShader *shader)
{
	if(m_state.shader) {
		m_state.shader->release();
	}
	m_state.shader = shader;
}

XShader *XBatch::getShader() const
{
	if(m_state.shader) {
		m_state.shader->addRef();
	}
	return m_state.shader;
}

void XBatch::setTexture(XTexture *texture)
{
	if(m_state.texture) {
		m_state.texture->release();
	}
	m_state.texture = texture;
}

XTexture *XBatch::getTexture() const
{
	if(m_state.texture) {
		m_state.texture->addRef();
	}
	return m_state.texture;
}

void XBatch::setPrimitive(PrimitiveType primitive)
{
	m_state.primitive = primitive;
}

XBatch::PrimitiveType XBatch::getPrimitive() const
{
	return m_state.primitive;
}

void XBatch::setVertexBuffer(XVertexBuffer *buffer)
{
	// Get texture draw order
	if(m_drawOrderMap.find(m_state.texture) == m_drawOrderMap.end())
	{
		m_state.drawOrder = m_drawOrderMap[m_state.texture] = m_drawOrderMap.size();
	}
	else
	{
		m_state.drawOrder = m_drawOrderMap[m_state.texture];
	}
	
	if(m_buffers.find(m_state) != m_buffers.end())
	{
		m_buffers[m_state]->release();
	}
	m_buffers[m_state] = buffer;
}

void XBatch::addVertices(XVertex *vertices, int vcount, uint *indices, int icount)
{
	// Get texture draw order
	if(m_drawOrderMap.find(m_state.texture) == m_drawOrderMap.end())
	{
		m_state.drawOrder = m_drawOrderMap[m_state.texture] = m_drawOrderMap.size();
	}
	else
	{
		m_state.drawOrder = m_drawOrderMap[m_state.texture];
	}

	XVertexBuffer *buffer;
	if(m_buffers.find(m_state) == m_buffers.end())
	{
		// Create new vertex buffer for this state
		buffer = m_buffers[m_state] = new XVertexBuffer(XVertexFormat::s_vct);
	}
	else
	{
		buffer = m_buffers[m_state];
	}
	
	buffer->addVertices(vertices, vcount, indices, icount);

	//vertex.position = m_matrixStack.top() * Vector4(vertex.position.x, vertex.position.y, 0.0f, 1.0f);
}

void XBatch::addVerticesAS(XScriptArray *vertices, XScriptArray *indices)
{
	XVertex *vx = new XVertex[vertices->GetSize()];
	for(int i = 0; i < vertices->GetSize(); i++)
	{
		vx[i] = *(XVertex*)vertices->At(i);
	}
	addVertices(vx, vertices->GetSize(), (uint*)indices->At(0), indices->GetSize());
	vertices->Release();
	indices->Release();
	delete[] vx;
}

void XBatch::draw()
{
	XGraphics::s_this->renderBatch(*this);
}

void XBatch::clear()
{
	for(StateVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		itr->second->release();
	}
	m_buffers.clear();
	m_drawOrderMap.clear();
	setTexture(0);
	setShader(0);
	setBlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
}

void XBatch::renderToTexture(XTexture *texture)
{
	if(texture)
	{
		if(!m_fbo)
		{
			m_fbo = XGraphics::CreateFrameBufferObject();
		}

		m_fbo->bind(texture);
		draw();
		m_fbo->unbind();
		texture->release();
	}
	else
	{
		LOG("Batch::renderToTexture(): Cannot render to 'null' texture.");
	}
}