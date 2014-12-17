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

namespace xd
{

Batch::Batch() :
	m_renderTarget(nullptr)
{
}

Batch::~Batch()
{
	delete m_renderTarget;
	clear();
}

void Batch::setBlendFunc(const BlendFunc src, const BlendFunc dst)
{
	m_state.srcBlendFunc = src;
	m_state.dstBlendFunc = dst;
}

void Batch::setProjectionMatrix(const Matrix4 &projmat)
{
	while(!m_matrixStack.empty()) m_matrixStack.pop();
	m_matrixStack.push(projmat);
	m_state.projMat = projmat;
}

Matrix4 Batch::getProjectionMatrix() const
{
	if(m_matrixStack.empty()) return Matrix4();
	return m_matrixStack.top();
}

void Batch::pushMatrix(const Matrix4 &mat)
{
	if(m_matrixStack.empty()) m_matrixStack.push(mat);
	else m_matrixStack.push(m_matrixStack.top() * mat);
	m_state.projMat = m_matrixStack.top();
}

void Batch::popMatrix()
{
	if(m_matrixStack.empty()) return;
	m_matrixStack.pop();
	m_state.projMat = !m_matrixStack.empty() ? m_matrixStack.top() : Matrix4();
}

void Batch::setShader(ShaderPtr shader)
{
	m_state.shader = shader;
}

ShaderPtr Batch::getShader() const
{
	return m_state.shader;
}

void Batch::setTexture(const xd::Texture2DPtr &texture)
{
	m_state.texture = texture;
}

xd::Texture2DPtr Batch::getTexture() const
{
	return m_state.texture;
}

void Batch::setPrimitive(PrimitiveType primitive)
{
	m_state.primitive = primitive;
}

Batch::PrimitiveType Batch::getPrimitive() const
{
	return m_state.primitive;
}

void Batch::addVertexBuffer(const shared_ptr<VertexBuffer> &buffer)
{
	if(!buffer) return;

	// Add buffer
	VertexBufferState vbs;
	vbs.buffer = buffer;
	vbs.state = m_state;
	m_buffers.push_back(vbs);
}

void Batch::addVertices(Vertex *vertices, int vcount, uint *indices, int icount)
{
	// Get texture draw order
	if(m_buffers.empty() || m_prevState != m_state)
	{
		VertexBufferState vbs;
		vbs.buffer = shared_ptr<VertexBuffer>(new VertexBuffer());
		vbs.state = m_state;
		m_buffers.push_back(vbs);
		m_prevState = m_state;
	}

	// Add vertices
	m_buffers.back().buffer->addVertices(vertices, vcount, indices, icount);

	//vertex.position = m_matrixStack.top() * Vector4(vertex.position.x, vertex.position.y, 0.0f, 1.0f);
}

void Batch::clear()
{
	m_buffers.clear();
	setProjectionMatrix(Matrix4());
	setTexture(0);
	setShader(0);
	setBlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
}

void Batch::renderToTexture(Texture2DPtr texture)
{
	if(texture)
	{
		if(!m_renderTarget)
		{
			m_renderTarget = new RenderTarget2D();
		}

		m_renderTarget->bind(texture);
		Graphics::renderBatch(*this);
		m_renderTarget->unbind();
	}
	else
	{
		LOG("Batch::renderToTexture(): Cannot render to 'null' texture.");
	}
}

}