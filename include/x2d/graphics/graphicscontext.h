#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "../engine.h"

namespace xd {

class RenderTarget2D;
class Batch;

class XDAPI GraphicsContext
{
public:
	GraphicsContext();

	enum Capability
	{
		BLEND = GL_BLEND,
		ALPHA_TEST = GL_ALPHA_TEST,
		DEPTH_TEST = GL_DEPTH_TEST,
		SCISSOR_TEST = GL_SCISSOR_TEST,
		LINE_SMOOTH = GL_LINE_SMOOTH,
		POINT_SMOOTH = GL_POINT_SMOOTH,
		POLYGON_SMOOTH = GL_POLYGON_SMOOTH,
		MULTISAMPLE = GL_MULTISAMPLE,
		TEXTURE_1D = GL_TEXTURE_1D,
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_3D = GL_TEXTURE_3D
	};

	//void clear(const uint mask, const Color fillColor);

	void enable(const Capability cap);
	void disable(const Capability cap);

	void resizeViewport(const uint w, const uint h);

	void setRenderTarget(RenderTarget2D *renderTarget);
	
	void renderBatch(const Batch &batch);
	
	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

public:
	uint m_width, m_height;
	RenderTarget2D *m_renderTarget;
};

}

#endif // GRAPHICS_CONTEXT_H