//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "batch.h"
#include "texture.h"
#include "vertexbufferobject.h"

#include <x2d/graphics.h>

AS_REG_VALUE(Vertex)

int Vertex::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectProperty("Vertex", "Vector2 position", offsetof(Vertex, position)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vertex", "Vector4 color", offsetof(Vertex, color)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vertex", "Vector2 texCoord", offsetof(Vertex, texCoord)); AS_ASSERT

	return r;
}

AS_REG_REF(Batch)

int Batch::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Batch", asBEHAVE_FACTORY, "Batch @f()", asFUNCTIONPR(Factory, (), Batch*), asCALL_CDECL); AS_ASSERT
		
		r = scriptEngine->RegisterObjectMethod("Batch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(Batch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setShader(Shader @shader)", asMETHOD(Batch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setTexture(Texture @texture)", asMETHOD(Batch, setTexture), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void addVertices(array<Vertex> @vertices, array<uint> @indices)", asMETHOD(Batch, addVerticesAS), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Vertex getVertex(int index)", asMETHOD(Batch, getVertex), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void modifyVertex(int index, Vertex vertex)", asMETHOD(Batch, modifyVertex), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Batch", "void draw()", asMETHOD(Batch, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void clear()", asMETHOD(Batch, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void makeStatic()", asMETHOD(Batch, makeStatic), asCALL_THISCALL); AS_ASSERT

	return r;
}

Batch::Batch() :
	m_static(false),
	m_texture(0),
	m_shader(0)
{
}

Batch::~Batch()
{
	if(m_texture) {
		m_texture->release();
	}
	for(TextureVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		delete itr->second.vbo;
	}
}

void Batch::setProjectionMatrix(const Matrix4 &projmat)
{
	m_projMatrix = projmat;
}

Matrix4 Batch::getProjectionMatrix() const
{
	return m_projMatrix;
}

void Batch::setShader(Shader *shader)
{
	m_shader = shader;
}

void Batch::setTexture(Texture *texture)
{
	if(m_texture) {
		m_texture->release();
	}
	m_texture = texture;
}

void Batch::addVertices(Vertex *vertices, int vertcount, uint *indices, int icount)
{
	if(m_static) {
		LOG("Cannot add vertices to a static Batch.");
		return;
	}

	VertexBuffer &buffer = m_buffers[m_texture];
	int ioffset = buffer.vertices.size();
	
	for(int i = 0; i < vertcount; i++) {
		buffer.vertices.push_back(vertices[i]);
	}
	
	for(int i = 0; i < icount; i++) {
		buffer.indices.push_back(indices[i] + ioffset);
	}
}

void Batch::addVerticesAS(Array *vertices, Array *indices)
{
	if(m_static) {
		LOG("Cannot add vertices to a static Batch.");
		return;
	}

	VertexBuffer &buffer = m_buffers[m_texture];
	int ioffset = buffer.vertices.size();
	
	for(uint i = 0; i < vertices->GetSize(); i++) {
		buffer.vertices.push_back(*(Vertex*)vertices->At(i));
	}
	
	for(uint i = 0; i < indices->GetSize(); i++) {
		buffer.indices.push_back(*(uint*)indices->At(i) + ioffset);
	}

	vertices->Release();
	indices->Release();
}

void Batch::modifyVertex(int index, Vertex vertex)
{
	if(index < 0 || index >= (int)m_buffers[m_texture].vertices.size()) {
		LOG("Batch.modifyVertex: Index out-of-bounds.");
		return;
	}

	m_buffers[m_texture].vertices[index] = vertex;
	if(m_static) {
		m_buffers[m_texture].vbo->uploadSub(index, &vertex, 1);
	}
}

Vertex Batch::getVertex(int index)
{
	if(index < 0 || index >= (int)m_buffers[m_texture].vertices.size()) {
		LOG("Batch.getVertex: Index out-of-bounds.");
		return Vertex();
	}

	return m_buffers[m_texture].vertices[index];
}

void Batch::draw()
{
	xdGraphics::s_this->renderBatch(*this);
}

void Batch::clear()
{
	m_buffers.clear();
}

Texture *Batch::renderToTexture()
{
	//m_fbo->bind();
	draw();
	//m_fbo->unbind();
	return 0;//m_fbo->getTexture();
}

void Batch::makeStatic()
{
	for(TextureVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		itr->second.vbo = xdGraphics::CreateVertexBufferObject();
		itr->second.vbo->upload(itr->second);
	}
	m_static = true;
}

bool Batch::isStatic() const
{
	return m_static;
}