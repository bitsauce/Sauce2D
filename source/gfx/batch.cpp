//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "sprite.h"
#include "shader.h"

#include <x2d/graphics/batch.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics/vertexbufferobject.h>
#include <x2d/graphics/framebufferobject.h>

#include <x2d/graphics.h>
#include <x2d/exception.h>

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

	r = scriptEngine->RegisterObjectBehaviour("Batch", asBEHAVE_FACTORY, "Batch @f()", asFUNCTIONPR(Factory, (), Batch*), asCALL_CDECL); AS_ASSERT
		
	// Getters/setters
	r = scriptEngine->RegisterObjectMethod("Batch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(Batch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setShader(Shader @shader)", asMETHOD(Batch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setTexture(Texture @texture)", asMETHOD(Batch, setTexture), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void setBlendFunc(const BlendFunc, const BlendFunc)", asMETHOD(Batch, setBlendFunc), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Matrix4 getProjectionMatrix() const", asMETHOD(Batch, getProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Shader @getShader() const", asMETHOD(Batch, getShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Texture @getTexture() const", asMETHOD(Batch, getTexture), asCALL_THISCALL); AS_ASSERT

	// Vertex data
	r = scriptEngine->RegisterObjectMethod("Batch", "void addVertices(array<Vertex> @vertices, array<uint> @indices)", asMETHOD(Batch, addVerticesAS), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "Vertex getVertex(int index)", asMETHOD(Batch, getVertex), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void modifyVertex(int index, Vertex vertex)", asMETHOD(Batch, modifyVertex), asCALL_THISCALL); AS_ASSERT

	// Misc
	r = scriptEngine->RegisterObjectMethod("Batch", "void draw()", asMETHOD(Batch, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void clear()", asMETHOD(Batch, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void makeStatic()", asMETHOD(Batch, makeStatic), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Batch", "void renderToTexture(Texture@)", asMETHOD(Batch, renderToTexture), asCALL_THISCALL); AS_ASSERT

	return r;
}

AS_REG_REF(SpriteBatch)

int SpriteBatch::Register(asIScriptEngine *scriptEngine)
{
	
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("SpriteBatch", asBEHAVE_FACTORY, "SpriteBatch @f()", asFUNCTIONPR(Factory, (), SpriteBatch*), asCALL_CDECL); AS_ASSERT
		
	// Getters/setters
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(SpriteBatch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setShader(Shader @shader)", asMETHOD(SpriteBatch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setTexture(Texture @texture)", asMETHOD(SpriteBatch, setTexture), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setBlendFunc(const BlendFunc, const BlendFunc)", asMETHOD(SpriteBatch, setBlendFunc), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Matrix4 getProjectionMatrix() const", asMETHOD(SpriteBatch, getProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Shader @getShader() const", asMETHOD(SpriteBatch, getShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Texture @getTexture() const", asMETHOD(SpriteBatch, getTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void add(Sprite @)", asMETHOD(SpriteBatch, add), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Sprite @get(int)", asMETHOD(SpriteBatch, get), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "int getSize() const", asMETHOD(SpriteBatch, getSize), asCALL_THISCALL); AS_ASSERT
	
	// Misc
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void draw()", asMETHOD(SpriteBatch, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void clear()", asMETHOD(SpriteBatch, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void makeStatic()", asMETHOD(SpriteBatch, makeStatic), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void renderToTexture(Texture@)", asMETHOD(SpriteBatch, renderToTexture), asCALL_THISCALL); AS_ASSERT

	return r;
}

Batch::Batch() :
	m_static(false),
	m_fbo(0)
{
}

Batch::~Batch()
{
	delete m_fbo;
	clear();
}

void Batch::setBlendFunc(const BlendFunc src, const BlendFunc dst)
{
	m_state.srcBlendFunc = src;
	m_state.dstBlendFunc = dst;
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
	if(m_state.shader) {
		m_state.shader->release();
	}
	m_state.shader = shader;
}

Shader *Batch::getShader() const
{
	if(m_state.shader) {
		m_state.shader->addRef();
	}
	return m_state.shader;
}

void Batch::setTexture(Texture *texture)
{
	if(m_state.texture) {
		m_state.texture->release();
	}
	m_state.texture = texture;
}

Texture *Batch::getTexture() const
{
	if(m_state.texture) {
		m_state.texture->addRef();
	}
	return m_state.texture;
}

void Batch::addVertices(Vertex *vertices, int vcount, uint *indices, int icount)
{
	if(m_static) {
		LOG("Cannot add vertices to a static Batch.");
		return;
	}
	
	// Get texture draw order
	if(m_drawOrderMap.find(m_state.texture) == m_drawOrderMap.end())
	{
		m_state.drawOrder = m_drawOrderMap[m_state.texture] = m_drawOrderMap.size();
	}else{
		m_state.drawOrder = m_drawOrderMap[m_state.texture];
	}

	VertexBuffer *buffer;
	if(m_buffers.find(m_state) == m_buffers.end())
	{
		// Create new vertex buffer for this state
		buffer = m_buffers[m_state] = new VertexBuffer();
	}else{
		buffer = m_buffers[m_state];
	}
	
	int ioffset = buffer->vertices.size();

	for(int i = 0; i < vcount; i++) {
		Vertex &vertex = vertices[i];
		//vertex.position = m_matrixStack.top() * Vector4(vertex.position.x, vertex.position.y, 0.0f, 1.0f);
		buffer->vertices.push_back(vertex);
	}
	
	for(int i = 0; i < icount; i++) {
		buffer->indices.push_back(indices[i] + ioffset);
	}
}

void Batch::addVerticesAS(Array *asvertices, Array *asindices)
{
	uint vcount = asvertices->GetSize();
	Vertex *vertices = new Vertex[vcount];
	for(uint i = 0; i < vcount; i++) {
		vertices[i] = *(Vertex*)asvertices->At(i);
	}

	uint icount = asindices->GetSize();
	uint *indices = new uint[icount];
	for(uint i = 0; i < icount; i++) {
		indices[i] = *(uint*)asindices->At(i);
	}

	addVertices(vertices, vcount, indices, icount);

	asvertices->Release();
	asindices->Release();
}

void Batch::modifyVertex(int index, Vertex vertex)
{
	if(index < 0 || index >= (int)m_buffers[m_state]->vertices.size()) {
		LOG("Batch.modifyVertex: Index out-of-bounds.");
		return;
	}

	m_buffers[m_state]->vertices[index] = vertex;
	if(m_static) {
		m_buffers[m_state]->vbo->uploadSub(index, &vertex, 1);
	}
}

Vertex Batch::getVertex(int index)
{
	if(index < 0 || index >= (int)m_buffers[m_state]->vertices.size()) {
		LOG("Batch.getVertex: Index out-of-bounds.");
		return Vertex();
	}

	return m_buffers[m_state]->vertices[index];
}

void Batch::draw()
{
	xdGraphics::s_this->renderBatch(*this);
}

void Batch::clear()
{
	for(StateVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		delete itr->second->vbo;
		delete itr->second;
	}
	m_buffers.clear();
	m_drawOrderMap.clear();
	m_static = false;
	setTexture(0);
	setShader(0);
	setBlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
}

void Batch::renderToTexture(Texture *texture)
{
	if(!texture) {
		LOG("Batch.renderToTexture: Cannot render to 'null' texture.");
		return;
	}

	if(!m_fbo) {
		m_fbo = xdGraphics::CreateFrameBufferObject();
	}

	m_fbo->bind(texture);
	draw();
	m_fbo->unbind();
}

void Batch::makeStatic()
{
	if(!xdGraphics::IsSupported(xdGraphics::VertexBufferObjects)) {
		AS_THROW("Tried to create a VBO whilst its not supported by the GPU!",);
	}
	for(StateVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		itr->second->vbo = xdGraphics::CreateVertexBufferObject();
		itr->second->vbo->upload(itr->second);
	}
	m_static = true;
}

bool Batch::isStatic() const
{
	return m_static;
}

SpriteBatch::~SpriteBatch()
{
	clear();
}

void SpriteBatch::add(Sprite *sprite)
{
	if(!m_static) {
		m_sprites.push_back(sprite);
	}
}

Sprite *SpriteBatch::get(int index)
{
	Sprite *sprite = m_sprites[index];
	sprite->addRef();
	m_returnedSprites.push_back(sprite);
	return sprite;
}

int SpriteBatch::getSize() const
{
	return m_sprites.size();
}

void SpriteBatch::draw()
{
	if(m_static)
	{
		// Assuming the returned sprite was modified,
		// let's re-upload it
		for(uint i = 0; i < m_returnedSprites.size(); i++)
		{
			Sprite *sprite = m_returnedSprites[i];

			// Get vertices
			Vertex vertices[4];
			sprite->getVertices(vertices);
			// m_matrixStack.top() * vertices[i];

			// Set state texture
			Texture *texture = sprite->getTexture();
			texture->addRef();
			setTexture(texture);

			// Set correct draw order
			m_state.drawOrder = m_drawOrderMap[texture];

			// Replace existing vertices
			m_buffers[m_state]->vbo->uploadSub(m_offsets[sprite], vertices, 4);
			texture->release();
		}
		m_returnedSprites.clear();
	}else{
		// Draw all the sprites
		for(uint i = 0; i < m_sprites.size(); i++)
		{
			this->addRef();
			m_sprites[i]->draw(this);
		}
	}
	Batch::draw();
}

void SpriteBatch::clear()
{
	for(uint i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->release();
	}
	m_returnedSprites.clear();
	m_sprites.clear();
	m_offsets.clear();
	Batch::clear();
}

void SpriteBatch::makeStatic()
{
	if(!xdGraphics::IsSupported(xdGraphics::VertexBufferObjects)) {
		AS_THROW("Tried to create a VBO whilst its not supported by the GPU!",);
	}

	for(uint i = 0; i < m_sprites.size(); i++)
	{
		Sprite *sprite = m_sprites[i];

		// Get sprite texture
		Texture *texture = sprite->getTexture();
		texture->addRef();
		setTexture(texture);
		
		// Store current vertex offset
		m_offsets[sprite] = m_buffers.find(m_state) == m_buffers.end() ? 0 : m_buffers[m_state]->vertices.size();
		texture->release();

		// Draw this sprite into the buffer
		this->addRef();
		sprite->draw(this);
	}
	for(StateVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		itr->second->vbo = xdGraphics::CreateVertexBufferObject();
		itr->second->vbo->upload(itr->second);
	}
	m_static = true;
}