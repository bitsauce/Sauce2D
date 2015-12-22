#ifndef CGF_GRAPHICS_H
#define CGF_GRAPHICS_H

#include <CGF/Math.h>
#include <CGF/Graphics/GraphicsContext.h>
#include <CGF/Graphics/BlendState.h>
#include <CGF/Graphics/Animation.h>
#include <CGF/Graphics/Spritebatch.h>
#include <CGF/Graphics/Font.h>
#include <CGF/Graphics/RenderTarget.h>
#include <CGF/Graphics/Pixmap.h>
#include <CGF/Graphics/Shader.h>
#include <CGF/Graphics/Shape.h>
#include <CGF/Graphics/Sprite.h>
#include <CGF/Graphics/Texture.h>
#include <CGF/Graphics/TextureAtlas.h>
#include <CGF/Graphics/Textureregion.h>
#include <CGF/Graphics/Vertex.h>
#include <CGF/Graphics/Vertexbuffer.h>
#include <CGF/Graphics/Viewport.h>


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

BEGIN_CGF_NAMESPACE

CGF_API extern uint QUAD_INDICES[6];
CGF_API extern Vector4 QUAD_VERTICES[4];
CGF_API extern Vector2 QUAD_TEXCOORD[4];

class CGF_API Graphics
{
	friend class Engine;
	friend class Window;
	friend class GraphicsContext;
public:
	Graphics(Window *window);
	~Graphics();

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

	// Swap buffers
	static void swapBuffers();


	SDL_GLContext *getSDLHandle() const;

protected:
	SDL_GLContext m_context;

	static int s_vsync;

	GraphicsContext s_graphicsContext;
};

END_CGF_NAMESPACE

#endif // CGF_GRAPHICS_H
