#ifndef X2D_GRAPHICS_H
#define X2D_GRAPHICS_H

#include "config.h"
#include "graphics/animation.h"
#include "graphics/batch.h"
#include "graphics/font.h"
#include "graphics/framebufferobject.h"
#include "graphics/pixmap.h"
#include "graphics/shader.h"
#include "graphics/shape.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "graphics/textureatlas.h"
#include "graphics/textureregion.h"
#include "graphics/vertex.h"
#include "graphics/vertexbuffer.h"
#include "graphics/vertexbufferobject.h"
#include "graphics/viewport.h"

/*********************************************************************
**	Graphics														**
**********************************************************************/

class XTexture;
class XShader;
class XFrameBufferObject;
class XVertexBufferObject;

XDAPI extern uint QUAD_INDICES[6];
XDAPI extern Vector4 QUAD_VERTICES[4];
XDAPI extern Vector2 QUAD_TEXCOORD[4];

class XDAPI XGraphics
{
	friend class XEngine;
	friend class XBatch;
	friend class XWindow;

	SINGLETON_DECL(XGraphics)

public:
	XGraphics();
	~XGraphics();

	void init(XWindow *window);

	enum Feature
	{
		VertexBufferObjects,
		FrameBufferObjects
	};

	// Rendering context
	void createContext();
	void destroyContext();

	// Orthographic projection
	void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f);
	void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f);
	
	// Viewports
	void setViewport(const Recti&);
	void setViewport(const Vector2i&, Vector2i&);
	void setViewport(const int, const int, const int, const int);
	void getViewport(int &x, int &y, int &w, int &h);

	// Refresh rate
	void setRefreshRate(const int hz);
	int getRefreshRate() const;
	
	// Vsync
	void enableVsync();
	void disableVsync();

	// Wireframe
	void enableWireframe();
	void disableWireframe();

	// Alpha blending
	void enableAlphaBlending();
	void disableAlphaBlending();

	// Time step & fps
	float getTimeStep() const;
	float getFPS() const;

	// Swap buffers
	void swapBuffers();

	static void SetOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f) { SINGLETON_OBJECT->setOrthoProjection(l, r, b, t, n, f); }
	static void GetOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f) { SINGLETON_OBJECT->getOrthoProjection(l, r, b, t, n, f); }
	static void SetViewport(const Recti &r) { SINGLETON_OBJECT->setViewport(r); }
	static void GetViewport(int &x, int &y, int &w, int &h) { SINGLETON_OBJECT->getViewport(x, y, w, h); }

protected:
	HGLRC m_context;
	HDC m_deviceContext;
	float m_currentOrtho[6];
	int m_currentViewport[4];

	float m_framesPerSecond;
	int m_refreshRate;
	float m_timeStep;

	void renderBatch(const XBatch &batch);
	bool isSupported(Feature feature);
};

#endif // X2D_GRAPHICS_H