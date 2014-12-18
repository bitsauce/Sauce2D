#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "../engine.h"

class RenderTarget2D;

class GraphicsDevice
{
public:
	GraphicsDevice();


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

	void enable(const Capability cap);
	void disable(const Capability cap);

	void setRenderTarget(const RenderTarget2D *renderTarget);
	
	// Orthographic projection
	void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f);
	void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f);
	
	// Viewports
	void setViewport(const Recti&);
	void setViewport(const Vector2i&, Vector2i&);
	void setViewport(const int, const int, const int, const int);
	void getViewport(int &x, int &y, int &w, int &h);
};

#endif // GRAPHICS_DEVICE_H