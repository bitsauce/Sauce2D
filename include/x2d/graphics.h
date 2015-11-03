#include <x2d/config.h>
#include <x2d/graphics/graphicsContext.h>
#include <x2d/graphics/blendState.h>
#include <x2d/graphics/animation.h>
#include <x2d/graphics/spritebatch.h>
#include <x2d/graphics/font.h>
#include <x2d/graphics/rendertarget.h>
#include <x2d/graphics/pixmap.h>
#include <x2d/graphics/shader.h>
#include <x2d/graphics/shape.h>
#include <x2d/graphics/sprite.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics/textureatlas.h>
#include <x2d/graphics/textureregion.h>
#include <x2d/graphics/vertex.h>
#include <x2d/graphics/vertexbuffer.h>
#include <x2d/graphics/viewport.h>

#ifndef X2D_GRAPHICS_H
#define X2D_GRAPHICS_H

#define GL_CHECK_ERROR \
	{ \
		GLenum error; \
		if((error = glGetError()) != GL_NO_ERROR) \
		{ \
			THROW("glGetError() returned 0x%X", error); \
		} \
	}

/*********************************************************************
**	Graphics class [static]											**
**********************************************************************/

BEGIN_XD_NAMESPACE

XDAPI extern uint QUAD_INDICES[6];
XDAPI extern Vector4 QUAD_VERTICES[4];
XDAPI extern Vector2 QUAD_TEXCOORD[4];

class XDAPI Graphics
{
	friend class Engine;
	friend class Window;
	friend class GraphicsContext;
public:

	enum Feature
	{
		VertexBufferObjects,
		FrameBufferObjects
	};

	// Vsync
	static void setVsync(const int mode);
	static int getVsync();

	// Wireframe
	static void enableWireframe();
	static void disableWireframe();

	// Time step & fps
	static double getFPS();

	// Swap buffers
	static void swapBuffers();

protected:
	static void init();
	static void clear();

	static double s_framesPerSecond;

	static ShaderPtr s_defaultShader;
	static Texture2DPtr s_defaultTexture;
	static GLuint s_vao;
	static GLuint s_vbo;
	static GLuint s_ibo;
	static int s_vsync;

	static GraphicsContext s_graphicsContext;
};

END_XD_NAMESPACE

#endif // X2D_GRAPHICS_H