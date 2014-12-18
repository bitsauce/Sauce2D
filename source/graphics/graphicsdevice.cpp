#include <x2d/graphics.h>

GraphicsDevice::GraphicsDevice()
{
}

void GraphicsDevice::enable(const Capability cap)
{
	glEnable(cap);
}

void GraphicsDevice::disable(const Capability cap)
{
	glDisable(cap);
}

void GraphicsDevice::setRenderTarget(const RenderTarget2D *renderTarget)
{
}
	
// Orthographic projection
void GraphicsDevice::setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)
{
}

void GraphicsDevice::getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f)
{
}
	
// Viewports
void GraphicsDevice::setViewport(const Recti&)
{
}

void GraphicsDevice::setViewport(const Vector2i&, Vector2i&)
{
}

void GraphicsDevice::setViewport(const int, const int, const int, const int)
{
}

void GraphicsDevice::getViewport(int &x, int &y, int &w, int &h)
{
}