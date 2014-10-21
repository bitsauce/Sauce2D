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

AS_REG_REF(XSpriteBatch, "SpriteBatch")

int XSpriteBatch::Register(asIScriptEngine *scriptEngine)
{
	
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("SpriteBatch", asBEHAVE_FACTORY, "SpriteBatch @f()", asFUNCTIONPR(Factory, (), XSpriteBatch*), asCALL_CDECL); AS_ASSERT
		
	// Getters/setters
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setProjectionMatrix(const Matrix4 &in)", asMETHOD(XSpriteBatch, setProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setShader(Shader @shader)", asMETHOD(XSpriteBatch, setShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setTexture(Texture @texture)", asMETHOD(XSpriteBatch, setTexture), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void setBlendFunc(const BlendFunc, const BlendFunc)", asMETHOD(XSpriteBatch, setBlendFunc), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Matrix4 getProjectionMatrix() const", asMETHOD(XSpriteBatch, getProjectionMatrix), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Shader @getShader() const", asMETHOD(XSpriteBatch, getShader), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Texture @getTexture() const", asMETHOD(XSpriteBatch, getTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void add(Sprite@)", asMETHOD(XSpriteBatch, add), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "Sprite @get(int)", asMETHOD(XSpriteBatch, get), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "int getSize() const", asMETHOD(XSpriteBatch, getSize), asCALL_THISCALL); AS_ASSERT
	
	// Misc
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void draw()", asMETHOD(XSpriteBatch, draw), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void clear()", asMETHOD(XSpriteBatch, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void makeStatic()", asMETHOD(XSpriteBatch, makeStatic), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("SpriteBatch", "void renderToTexture(Texture@)", asMETHOD(XSpriteBatch, renderToTexture), asCALL_THISCALL); AS_ASSERT

	return r;
}

XSpriteBatch::~XSpriteBatch()
{
	clear();
}

void XSpriteBatch::add(XSprite *sprite)
{
	if(!m_static) {
		m_sprites.push_back(sprite);
	}
}

XSprite *XSpriteBatch::get(int index)
{
	XSprite *sprite = 0;
	if(index < m_sprites.size())
	{
		sprite = m_sprites[index];
		sprite->addRef();
		m_returnedSprites.push_back(sprite);
	}
	return sprite;
}

int XSpriteBatch::getSize() const
{
	return m_sprites.size();
}

void XSpriteBatch::draw()
{
	if(m_static)
	{
		// Assuming the returned sprite was modified,
		// let's re-upload it
		for(uint i = 0; i < m_returnedSprites.size(); i++)
		{
			XSprite *sprite = m_returnedSprites[i];

			// Get vertices
			XVertex vertices[4];
			sprite->getVertices(vertices);
			// m_matrixStack.top() * vertices[i];

			// Set state texture
			XTexture *texture = sprite->getTexture();
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
	XBatch::draw();
}

void XSpriteBatch::clear()
{
	for(uint i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->release();
	}
	m_returnedSprites.clear();
	m_sprites.clear();
	m_offsets.clear();
	XBatch::clear();
}

void XSpriteBatch::makeStatic()
{
	if(!XGraphics::IsSupported(XGraphics::VertexBufferObjects)) {
		AS_THROW("Tried to create a VBO whilst its not supported by the GPU!",);
	}

	for(uint i = 0; i < m_sprites.size(); i++)
	{
		XSprite *sprite = m_sprites[i];

		// Get sprite texture
		XTexture *texture = sprite->getTexture();
		if(texture) {
			texture->addRef();
		}
		setTexture(texture);
		
		// Store current vertex offset
		m_offsets[sprite] = m_buffers.find(m_state) == m_buffers.end() ? 0 : m_buffers[m_state]->vertices.size();
		if(texture) {
			texture->release();
		}

		// Draw this sprite into the buffer
		this->addRef();
		sprite->draw(this);
	}
	for(StateVertexMap::iterator itr = m_buffers.begin(); itr != m_buffers.end(); ++itr) {
		itr->second->vbo = XGraphics::CreateVertexBufferObject();
		itr->second->vbo->upload(itr->second);
	}
	m_static = true;
}