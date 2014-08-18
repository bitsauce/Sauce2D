#include <x2d/graphics/spritebatch.h>

#include <x2d/exception.h>
#include <x2d/graphics.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics/vertexbufferobject.h>
#include "sprite.h"

AS_REG_REF(SpriteBatch)

int SpriteBatch::Register(asIScriptEngine *scriptEngine)
{
	
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("SpriteBatch", asBEHAVE_FACTORY, "SpriteBatch @f()", asFUNCTIONPR(Factory, (), SpriteBatch*), asCALL_CDECL); AS_ASSERT
		
	// Getters/setters
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(SpriteBatch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setShader(Shader @shader)", asMETHOD(SpriteBatch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setTexture(Texture @texture)", asMETHOD(SpriteBatch, setTexture), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setBlendFunc(const BlendFunc, const BlendFunc)", asMETHOD(SpriteBatch, setBlendFunc), asCALL_THISCALL); AS_ASSERT
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

	// Serialization
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void serialize(StringStream&) const", asMETHOD(SpriteBatch, serialize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("SpriteBatch", asBEHAVE_FACTORY, "SpriteBatch @f(StringStream&)", asFUNCTIONPR(Factory, (StringStream&), SpriteBatch*), asCALL_CDECL); AS_ASSERT

	return r;
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
			if(texture) {
				texture->addRef();
			}
			setTexture(texture);

			// Set correct draw order
			m_state.drawOrder = m_drawOrderMap[texture];

			// Replace existing vertices
			m_buffers[m_state]->vbo->uploadSub(m_offsets[sprite], vertices, 4);
			if(texture) {
				texture->release();
			}
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

#include <x2d/scriptengine.h>

void SpriteBatch::serialize(StringStream &ss) const
{
	(stringstream&)ss << m_sprites.size() << endl;
	for(uint i = 0; i < m_sprites.size(); i++) {
		g_engine->getScriptEngine()->serialize((void*)&m_sprites[i], Sprite::GetTypeId() | asTYPEID_OBJHANDLE, ss);
	}
}

SpriteBatch *SpriteBatch::Factory(StringStream &ss)
{
	SpriteBatch *batch = new SpriteBatch();

	uint size;
	(stringstream&)ss >> size; ((stringstream&)ss).ignore();
	batch->m_sprites.resize(size);
	for(uint i = 0; i < size; i++) {
		g_engine->getScriptEngine()->deserialize(&batch->m_sprites[i], Sprite::GetTypeId() | asTYPEID_OBJHANDLE, ss);
	}
	return batch;
}