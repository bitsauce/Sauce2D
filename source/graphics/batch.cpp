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
	while(!m_matrixStack.empty()) m_matrixStack.pop();
	m_matrixStack.push(projmat);
	m_state.projMat = projmat;
}

Matrix4 XBatch::getProjectionMatrix() const
{
	if(m_matrixStack.empty()) return Matrix4();
	return m_matrixStack.top();
}

void XBatch::pushMatrix(const Matrix4 &mat)
{
	if(m_matrixStack.empty()) m_matrixStack.push(mat);
	else m_matrixStack.push(m_matrixStack.top() * mat);
	m_state.projMat = m_matrixStack.top();
}

void XBatch::popMatrix()
{
	if(m_matrixStack.empty()) return;
	m_matrixStack.pop();
	m_state.projMat = !m_matrixStack.empty() ? m_matrixStack.top() : Matrix4();
}

void XBatch::setShader(shared_ptr<XShader> shader)
{
	m_state.shader = shader;
}

shared_ptr<XShader> XBatch::getShader() const
{
	return m_state.shader;
}

void XBatch::setTexture(const shared_ptr<XTexture> &texture)
{
	m_state.texture = texture;
}

shared_ptr<XTexture> XBatch::getTexture() const
{
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

void XBatch::addVertexBuffer(const shared_ptr<XVertexBuffer> &buffer)
{
	if(!buffer) return;

	// Add buffer
	VertexBufferState vbs;
	vbs.buffer = buffer;
	vbs.state = m_state;
	m_buffers.push_back(vbs);
}

void XBatch::addVertices(XVertex *vertices, int vcount, uint *indices, int icount)
{
	// Get texture draw order
	if(m_buffers.empty() || m_prevState != m_state)
	{
		VertexBufferState vbs;
		vbs.buffer = shared_ptr<XVertexBuffer>(new XVertexBuffer());
		vbs.state = m_state;
		m_buffers.push_back(vbs);
		m_prevState = m_state;
	}

	// Add vertices
	m_buffers.back().buffer->addVertices(vertices, vcount, indices, icount);

	//vertex.position = m_matrixStack.top() * Vector4(vertex.position.x, vertex.position.y, 0.0f, 1.0f);
}

void XBatch::clear()
{
	m_buffers.clear();
	setProjectionMatrix(Matrix4());
	setTexture(0);
	setShader(0);
	setBlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
}

void XBatch::renderToTexture(shared_ptr<XTexture> texture)
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
	}
	else
	{
		LOG("XBatch::renderToTexture(): Cannot render to 'null' texture.");
	}
}