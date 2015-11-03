#ifndef X2D_BATCH_H
#define X2D_BATCH_H

#include <x2d/engine.h>
#include <x2d/graphics/vertex.h>
#include <x2d/graphics/vertexBuffer.h>
#include <x2d/graphics/blendState.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics/shader.h>
#include <x2d/graphics/font.h>

BEGIN_XD_NAMESPACE

class Sprite;

/*********************************************************************
**	Batch															**
**********************************************************************/
class XDAPI SpriteBatch
{
public:
	SpriteBatch(GraphicsContext &graphicsContext);
	~SpriteBatch();

	enum SpriteSortMode
	{
		BACK_TO_FRONT,
		DEFERRED,
		FRONT_TO_BACK,
		IMMEDIATE,
		TEXTURE
	};

	struct State
	{
		State(const SpriteSortMode mode = DEFERRED, const BlendState blendState = BlendState::PRESET_ALPHA_BLEND, const Matrix4 &projectionMatrix = Matrix4(), const ShaderPtr shader = nullptr) :
			mode(mode),
			blendState(blendState),
			projectionMatix(projectionMatrix),
			shader(shader)
		{
		}

		SpriteSortMode mode;
		BlendState blendState;
		Matrix4 projectionMatix;
		ShaderPtr shader;
	};

	void begin(const State &state = State());
	void drawSprite(const Sprite &sprite);
	void drawText(const Vector2 &pos, const string &text, const FontPtr font);
	void end();
	void flush();

	State getState() const { return m_state; }
	GraphicsContext &getGraphicsContext() const { return m_graphicsContext; }
	uint getTextureSwapCount() const;

private:

	// SpriteBatch state
	State m_state, m_prevState;
	Texture2DPtr m_prevTexture;

	// Set between begin() and end()
	bool m_beingCalled;

	// Vertex & index buffers
	Vertex *m_vertices;
	uint *m_indices;
	Sprite *m_sprites;
	uint m_spriteCount;

	// Graphics context
	GraphicsContext &m_graphicsContext;
};

END_XD_NAMESPACE

#endif // X2D_BATCH_H