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
	//if(m_state.shader) {
	//	m_state.shader->release();
	//}
	m_state.shader = shader;
}

XShader *XBatch::getShader() const
{
	//if(m_state.shader) {
	//	m_state.shader->addRef();
	//}
	return m_state.shader;
}

void XBatch::setTexture(XTexture *texture)
{
	//if(m_state.texture) {
	//	m_state.texture->release();
	//}
	m_state.texture = texture;
}

XTexture *XBatch::getTexture() const
{
	//if(m_state.texture) {
	//	m_state.texture->addRef();
	//}
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

void XBatch::setVertexBuffer(const XVertexBuffer &buffer)
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
	
	m_buffers[m_state].addVertices(vertices, vcount, indices, icount);

	//vertex.position = m_matrixStack.top() * Vector4(vertex.position.x, vertex.position.y, 0.0f, 1.0f);
}

void XBatch::clear()
{
	m_projMatrix.identity();
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
			m_fbo = new XFrameBufferObject();
		}

		m_fbo->bind(texture);
		XGraphics::renderBatch(*this);
		m_fbo->unbind();
		//texture->release();
	}
	else
	{
		LOG("XBatch::renderToTexture(): Cannot render to 'null' texture.");
	}
}