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

SpriteBatch::SpriteBatch(GraphicsContext &graphicsContext) : 
	m_graphicsContext(graphicsContext),
	m_beingCalled(false)
{
	m_sprites = new Sprite[2084];
	m_vertices = new Vertex[8912];
	m_indices = new uint[12288];
}

SpriteBatch::~SpriteBatch()
{
	delete[] m_sprites;
	delete[] m_vertices;
	delete[] m_indices;
}

void SpriteBatch::begin(const State &state)
{
	if(m_beingCalled)
	{
		LOG("SpriteBatch::begin(): called twice before end()");
		return;
	}

	m_beingCalled = true;
	m_spriteCount = 0;
	m_state = state;

	m_prevState.projectionMatix = m_graphicsContext.getProjectionMatrix();
	m_prevState.blendState = m_graphicsContext.getBlendState();
	m_prevState.shader = m_graphicsContext.getShader();
	m_prevTexture = m_graphicsContext.getTexture();
}

void SpriteBatch::drawSprite(const Sprite &sprite)
{
	if(!m_beingCalled)
	{
		LOG("SpriteBatch::drawSprite(): Called before begin()");
		return;
	}

	if(!sprite.getTexture())
	{
		LOG("SpriteBatch::drawSprite(): Sprite needs a texture.");
		return;
	}

	m_sprites[m_spriteCount++] = sprite;
}

void SpriteBatch::drawText(const Vector2 &pos, const string &text, const FontPtr font)
{
	if(!m_beingCalled)
	{
		LOG("SpriteBatch::drawText(): Called before begin()");
		return;
	}

	if(!font) return;

	font->draw(this, pos, text);
}



void SpriteBatch::end()
{
	if(!m_beingCalled)
	{
		LOG("SpriteBatch::end(): Called before begin()");
		return;
	}
	
	// Draw sprites
	if(m_spriteCount > 0)
	{
		switch(m_state.mode)
		{
			case DEFERRED:
			{
				// Sprites are not drawn until end() is called. end() will apply graphics
				// device settings and draw all the sprites in one batch, in the same
				// order calls to draw*() were recieved. This mode allows draw*() calls
				// to two or more instances of SpriteBatch without introducing conflicting
				// graphics device settings. SpriteBatch defaults to DEFERRED mode.

				m_graphicsContext.setProjectionMatrix(m_state.projectionMatix);
				m_graphicsContext.setBlendState(m_state.blendState);
				m_graphicsContext.setShader(m_state.shader);

				// Separate sprites by depth and texture
				map<float, map<Texture2DPtr, list<Sprite*>>> layerTextureMap;
				for(uint i = 0; i < m_spriteCount; ++i)
				{
					Sprite *sprite = &m_sprites[i];
					layerTextureMap[sprite->m_depth][sprite->m_texture].push_back(sprite);
				}

				// For each depth
				for(map<float, map<Texture2DPtr, list<Sprite*>>>::iterator itr1 = layerTextureMap.begin(); itr1 != layerTextureMap.end(); ++itr1)
				{
					// For each texture
					uint spriteCount = 0;
					Texture2DPtr prevTexture = itr1->second.begin()->first;
					for(map<Texture2DPtr, list<Sprite*>>::iterator itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2)
					{
						// Batch all sprite vertex data
						for(list<Sprite*>::iterator itr3 = itr2->second.begin(); itr3 != itr2->second.end(); ++itr3)
						{
							(*itr3)->getVertices(m_vertices + spriteCount * 4, m_indices + spriteCount * 6, spriteCount * 4);
							spriteCount++;
						}

						// If this texture is different from the previous one
						if(itr2->first != prevTexture)
						{
							// And if we have sprites
							if(spriteCount > 0)
							{
								// Draw textured primitives
								m_graphicsContext.setTexture(prevTexture);
								m_graphicsContext.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, m_vertices, spriteCount * 4, m_indices, spriteCount * 6);
								spriteCount = 0;
							}
							prevTexture = itr2->first;
						}
					}
					
					// If we have sprites
					if(spriteCount > 0)
					{
						// Draw remaining textured primitives
						m_graphicsContext.setTexture(prevTexture);
						m_graphicsContext.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, m_vertices, spriteCount * 4, m_indices, spriteCount * 6);
						spriteCount = 0;
					}
				}
			}
			break;

			case TEXTURE:
			{
				// Sort sprites by texture. Draw forwards and batch together
				// similar textures, as long as they don't overlap a different texture.
				/*uint prevIndex = 0;
				Texture2DPtr prevTexture = m_sprites[0].texture;
				for(uint i = 0; i < m_spriteCount; ++i)
				{
					if(m_sprites[i].texture != prevTexture)
					{
						m_graphicsContext.setTexture(m_sprites[i].texture);
						m_graphicsContext.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, m_vertices + prevIndex * 4, (i - prevIndex) * 4, m_indices + prevIndex * 6, (i - prevIndex) * 6);
						prevIndex = i;
					}
				}
				m_graphicsContext.setTexture(m_sprites[prevIndex].texture);
				m_graphicsContext.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, m_vertices + prevIndex * 4, (m_spriteCount - prevIndex) * 4, m_indices + prevIndex * 6, (m_spriteCount - prevIndex) * 6);*/
			}
			break;
		}
	}
		
	m_graphicsContext.setProjectionMatrix(m_prevState.projectionMatix);
	m_graphicsContext.setBlendState(m_prevState.blendState);
	m_graphicsContext.setShader(m_prevState.shader);
	m_graphicsContext.setTexture(m_prevTexture);
	
	m_beingCalled = false;
}

}