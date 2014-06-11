#ifndef GFX_OPENGL_H
#define GFX_OPENGL_H

#include <x2d/graphics.h>
#include <GL/glew.h>
#include <gl/wglew.h>

class Window;

/**
  *	\brief 
  */
class OpenGL : public xdGraphics
{
public:
	void init(Window *window);

	void createContext(HWND window);
	void destroyContext();

	void swapBuffers();

	void enableVsync();
	void disableVsync();

	void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f);
	void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f);
	
	void setViewport(const Recti&);
	void setViewport(const Vector2i&, Vector2i&);
	void setViewport(const int, const int, const int, const int);

private:
	void renderBatch(const Batch &batch);
	Texture *createTexture(const Pixmap &pixmap);
	Shader *createShader(const string &vertFilePath, const string &fragFilePath);
	VertexBufferObject *createVertexBufferObject();

	HGLRC m_openGLContext;
	HDC m_deviceContext;
	float m_currentOrtho[6];
};

#endif // GFX_OPENGL_H