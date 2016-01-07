//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/graphics.h>

BEGIN_CGF_NAMESPACE

uint QUAD_INDICES[6] = {
	0, 3, 2, 0, 2, 1
};

Vector4<float> QUAD_VERTICES[4] = {
	Vector4<float>(0.0f, 0.0f, 0.0f, 1.0f),
	Vector4<float>(1.0f, 0.0f, 0.0f, 1.0f),
	Vector4<float>(1.0f, 1.0f, 0.0f, 1.0f),
	Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f)
};

Vector2F QUAD_TEXCOORD[4] = {
	Vector2F(0.0f, 0.0f),
	Vector2F(1.0f, 0.0f),
	Vector2F(1.0f, 1.0f),
	Vector2F(0.0f, 1.0f)
};

// Standard position, color and texCoord vertex format
VertexFormat VertexFormat::s_vct;
/*
int Graphics::s_vsync = 0;

Graphics::~Graphics()
{
	glDeleteBuffers(1, &s_vbo);
	glDeleteVertexArrays(1, &s_vao);
	SDL_GL_DeleteContext(m_context);
}

void Graphics::swapBuffers()
{
	glfwSwapBuffers(Window::s_window);
}

SDL_GLContext * Graphics::getSDLHandle() const
{
	return m_context;
}

// Vsync
void Graphics::setVsync(const int mode)
{
	glfwSwapInterval(mode);
	SDL_GL_SetSwapInterval(mode);
	s_vsync = mode;
}

int Graphics::getVsync()
{
	return s_vsync;
}

// Wireframe
void Graphics::enableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Graphics::disableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
*/
END_CGF_NAMESPACE
