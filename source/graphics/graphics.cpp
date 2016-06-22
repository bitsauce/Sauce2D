//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2016 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

uint QUAD_INDICES[6] = {
	0, 3, 2, 0, 2, 1
};

Vector4F QUAD_VERTICES[4] = {
	Vector4F(0.0f, 0.0f, 0.0f, 1.0f),
	Vector4F(1.0f, 0.0f, 0.0f, 1.0f),
	Vector4F(1.0f, 1.0f, 0.0f, 1.0f),
	Vector4F(0.0f, 1.0f, 0.0f, 1.0f)
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
END_SAUCE_NAMESPACE
