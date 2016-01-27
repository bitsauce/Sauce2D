#ifndef CGF_BATCH_H
#define CGF_BATCH_H

#include <CGF/Common.h>
#include <CGF/Graphics/vertex.h>
#include <CGF/Graphics/vertexBuffer.h>
#include <CGF/Graphics/blendState.h>
#include <CGF/Graphics/texture.h>
#include <CGF/Graphics/shader.h>
#include <CGF/Graphics/font.h>

BEGIN_CGF_NAMESPACE

class Sprite;

/*********************************************************************
**	Batch															**
**********************************************************************/
class CGF_API SpriteBatch
{
public:
	SpriteBatch(GraphicsContext *graphicsContext, const uint maxSprites = 2048);
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
		State(const SpriteSortMode mode = DEFERRED, const BlendState blendState = BlendState::PRESET_ALPHA_BLEND, const Matrix4 &projectionMatrix = Matrix4(), Shader *shader = nullptr) :
			mode(mode),
			blendState(blendState),
			projectionMatix(projectionMatrix),
			shader(shader)
		{
		}

		SpriteSortMode mode;
		BlendState blendState;
		Matrix4 projectionMatix;
		Shader *shader;
	};

	void begin(const State &state = State());
	void drawSprite(const Sprite &sprite);
	void drawText(const Vector2F &pos, const string &text, Font *font);
	void end();
	void flush();

	State getState() const { return m_state; }
	GraphicsContext *getGraphicsContext() const { return m_graphicsContext; }
	uint getTextureSwapCount() const;

private:

	// SpriteBatch state
	State m_state, m_prevState;
	Texture2D *m_prevTexture;

	// Set between begin() and end()
	bool m_beingCalled;

	// Vertex & index buffers
	Vertex *m_vertices;
	uint *m_indices;
	Sprite *m_sprites;
	uint m_spriteCount;
	const uint m_maxSpriteCount;

	// Graphics context
	GraphicsContext *m_graphicsContext;
};

END_CGF_NAMESPACE

#endif // CGF_BATCH_H
