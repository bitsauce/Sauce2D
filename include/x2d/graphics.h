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
public:
	AS_DECL_SINGLETON

	XGraphics();
	virtual ~XGraphics() {}

	enum Feature
	{
		VertexBufferObjects,
		FrameBufferObjects
	};

	// Global factories
	static XTexture*			CreateTexture(const XPixmap &pixmap);
	static XTexture*			CreateTexture(const string &filePath);
	static XTexture*			CreateTexture(const int width, const int height);
	static XTexture*			CreateTexture(const XTexture &texture);
	static XShader*				CreateShader(const string &vertFilePath, const string &fragFilePath);
	static XVertexBufferObject*	CreateVertexBufferObject(const XVertexBuffer &buffer);
	static XFrameBufferObject*	CreateFrameBufferObject();
	static bool					IsSupported(Feature feature);

	// Refresh rate
	void setRefreshRate(const int hz);
	int getRefreshRate() const;

	// Vsync
	virtual void enableVsync()										{ NOT_IMPLEMENTED(enableVsync) }
	virtual void disableVsync()										{ NOT_IMPLEMENTED(disableVsync) }

	// Time step & fps
	float getTimeStep() const;
	float getFPS() const;

	// Swap buffers
	virtual void swapBuffers() = 0;

private:
	float m_framesPerSecond;
	int m_refreshRate;
	float m_timeStep;

	virtual void					renderBatch(const XBatch &batch) = 0;
	virtual XTexture*				createTexture(const XPixmap &pixmap) = 0;
	virtual XShader*				createShader(const string &vertFilePath, const string &fragFilePath) = 0;
	virtual XVertexBufferObject*	createVertexBufferObject(const XVertexBuffer &buffer) = 0;
	virtual XFrameBufferObject*		createFrameBufferObject() = 0;
	virtual bool					isSupported(Feature feature) = 0;

	static XGraphics *s_this;
};

#endif // X2D_GRAPHICS_H