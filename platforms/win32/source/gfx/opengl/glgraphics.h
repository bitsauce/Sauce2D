#ifndef GFX_OPENGL_H
#define GFX_OPENGL_H

#include <x2d/graphics.h>
#include <GL/glew.h>
#include <gl/wglew.h>

/**
  *	\brief 
  */
class OpenGL : public Graphics
{
private:
	void renderBatch(const Batch &batch);
	Texture *createTexture(const Pixmap &pixmap);
};

#endif // GFX_OPENGL_H