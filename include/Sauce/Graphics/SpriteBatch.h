#ifndef SAUCE_BATCH_H
#define SAUCE_BATCH_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/vertex.h>
#include <Sauce/Graphics/vertexBuffer.h>
#include <Sauce/Graphics/blendState.h>
#include <Sauce/Graphics/texture.h>
#include <Sauce/Graphics/shader.h>
#include <Sauce/Graphics/font.h>

BEGIN_SAUCE_NAMESPACE

class Sprite;

/*********************************************************************
**	Batch															**
**********************************************************************/
class SAUCE_API SpriteBatch
{
public:
	SpriteBatch(const uint maxSprites = 2048);
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
		State(const SpriteSortMode mode = DEFERRED, const BlendState blendState = BlendState::PRESET_ALPHA_BLEND, const Matrix4 &transformationMatix = Matrix4(), shared_ptr<Shader> shader = nullptr) :
			mode(mode),
			blendState(blendState),
			transformationMatix(transformationMatix),
			shader(shader)
		{
		}

		SpriteSortMode mode;
		BlendState blendState;
		Matrix4 transformationMatix;
		shared_ptr<Shader> shader;
	};

	void begin(GraphicsContext *graphicsContext, const State &state = State());
	void drawSprite(const Sprite &sprite);
	void drawText(const Vector2F &pos, const string &text, Font *font);
	void end();
	void flush();

	State getState() const { return m_state; }
	uint getTextureSwapCount() const;

private:

	// SpriteBatch state
	State m_state;

	// Vertex & index buffers
	Vertex *m_vertices;
	uint *m_indices;
	Sprite *m_sprites;
	uint m_spriteCount;
	const uint m_maxSpriteCount;
	GraphicsContext *m_graphicsContext;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_BATCH_H
