//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

namespace xd {

uint QUAD_INDICES[6] = {
	0, 3, 2, 0, 2, 1
};

Vector4 QUAD_VERTICES[4] = {
	Vector4(0.0f, 0.0f, 0.0f, 1.0f),
	Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	Vector4(0.0f, 1.0f, 0.0f, 1.0f)
};

Vector2 QUAD_TEXCOORD[4] = {
	Vector2(0.0f, 1.0f),
	Vector2(1.0f, 1.0f),
	Vector2(1.0f, 0.0f),
	Vector2(0.0f, 0.0f)
};

VertexFormat VertexFormat::s_vct;

HGLRC Graphics::s_context = 0;

float Graphics::s_framesPerSecond = 0.0f;
int Graphics::s_refreshRate;
float Graphics::s_timeStep;

GraphicsContext Graphics::s_graphicsContext;

void Graphics::setRefreshRate(const int hz)
{
	if(hz == 0)
	{
		LOG("Graphics::setRefreshRate() refresh rate cannot be 0");
		return;
	}
	s_timeStep = 1.0f/hz;
	s_refreshRate = hz;
}

int Graphics::getRefreshRate()
{
	return s_refreshRate;
}

float Graphics::getTimeStep()
{
	return s_timeStep;
}

float Graphics::getFPS()
{
	return s_framesPerSecond;
}

void Graphics::init()
{
	// Init glew
	//glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		assert("GLEW did not initialize!");
	}

	// Check if non-power of two textures are supported
	if(!GLEW_ARB_texture_non_power_of_two) {
		LOG("WARNING: NPOT is not supported on this card!");
	}

	// Check if FBOs are supported
	if(!GLEW_EXT_framebuffer_object) {
		LOG("WARNING: FBO is not supported on this card!");
	}

	// Check if PBOs are supported
	if(!GLEW_EXT_pixel_buffer_object) {
		LOG("WARNING: PBO is not supported on this card!");
	}

	// Check if (changing) v-sync (state) is supported
	if(!WGLEW_EXT_swap_control) {
		LOG("WARNING: VSYNC is not supported on this card!");
	}
	
	// Setup default vertex format
	VertexFormat::s_vct.set(VERTEX_POSITION, 2);
	VertexFormat::s_vct.set(VERTEX_COLOR, 4, XD_UBYTE);
	VertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);

	// Set refresh rate
	setRefreshRate(60);

	// Setup viewport
	Vector2i size = XWindow::getSize();
	s_graphicsContext.resizeViewport(size.x, size.y);
	XWindow::s_graphicsContext = &s_graphicsContext;

	// Init Graphics
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable alpha test // Optimization?
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LEQUAL, 0.0f);

	// Set Graphics hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(4);
}

void Graphics::createContext()
{
	// Create Graphics rendering context
	s_context = wglCreateContext(XWindow::s_deviceContext);

	// Make context current
	wglMakeCurrent(XWindow::s_deviceContext, s_context);
}

void Graphics::destroyContext()
{
	if(s_context)
	{
		// Make the rendering context not current
		wglMakeCurrent(NULL, NULL);

		// Delete the Graphics rendering context
		wglDeleteContext(s_context);

		// Set to null
		s_context = NULL;
	}
}

void Graphics::swapBuffers()
{
	SwapBuffers(XWindow::s_deviceContext);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool Graphics::isSupported(Feature feature)
{
	switch(feature)
	{
	case VertexBufferObjects: return GLEW_ARB_vertex_buffer_object == GL_TRUE; break;
	case FrameBufferObjects: return GLEW_ARB_framebuffer_object == GL_TRUE; break;
	}
	return false;
}

// Vsync
void Graphics::enableVsync()
{
	wglSwapIntervalEXT(1);
}

void Graphics::disableVsync()
{
	wglSwapIntervalEXT(0);
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

// Alpha blending
void Graphics::enableAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
	
void Graphics::disableAlphaBlending()
{
	glDisable(GL_BLEND);
}

}