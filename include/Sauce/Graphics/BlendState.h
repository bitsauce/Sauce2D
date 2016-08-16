#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API BlendState
{
	friend class GraphicsContext;
public:

	enum BlendFactor
	{
		BLEND_ZERO = GL_ZERO,
		BLEND_ONE = GL_ONE,
		BLEND_SRC_COLOR = GL_SRC_COLOR,
		BLEND_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
		BLEND_SRC_ALPHA = GL_SRC_ALPHA,
		BLEND_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		BLEND_DST_COLOR = GL_DST_COLOR,
		BLEND_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		BLEND_DST_ALPHA = GL_DST_ALPHA,
		BLEND_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		BLEND_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
	};

	enum Preset
	{
		PRESET_ALPHA_BLEND,
		PRESET_OPAQUE,
		PRESET_ADDITIVE,
		PRESET_MULTIPLY,
		PRESET_PREMULTIPLIED_ALPHA
	};

	BlendState(const Preset preset);
	BlendState(const BlendFactor src, const BlendFactor dst);
	BlendState(const BlendFactor csrc, const BlendFactor cdst, const BlendFactor asrc, const BlendFactor adst);

private:
	BlendFactor m_src, m_dst, m_alphaSrc, m_alphaDst;
};

END_SAUCE_NAMESPACE

#endif // BLEND_STATE_H