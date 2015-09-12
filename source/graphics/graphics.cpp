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

BEGIN_XD_NAMESPACE

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

/*float Graphics::getTimeStep()
{
	return s_timeStep;
}*/

float Graphics::getFPS()
{
	return s_framesPerSecond;
}

bool WGLExtensionSupported(const char *extension_name)
{
	// this is pointer to function which returns pointer to string with list of all wgl extensions
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

	// determine pointer to wglGetExtensionsStringEXT function
	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
	{
		// string was not found
		return false;
	}

	// extension is supported
	return true;
}

PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

void Graphics::init()
{
	// Initialize the GL3W library
	if(gl3wInit() != 0) {
		assert("GLEW did not initialize!");
	}
	
	// Print GPU info
	LOG("** Using GPU: %s (OpenGL %s) **", glGetString(GL_VENDOR), glGetString(GL_VERSION));

	// Check OpenGL 3.2 support
	if (!gl3wIsSupported(3, 2)) {
		assert("OpenGL 3.2 not supported\n");
	}

	// Get the vsync/swap_control EXT
	if (WGLExtensionSupported("WGL_EXT_swap_control"))
	{
		// Extension is supported, init pointers.
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		// this is another function from WGL_EXT_swap_control extension
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	}

	// Setup default vertex format
	VertexFormat::s_vct.set(VERTEX_POSITION, 2);
	VertexFormat::s_vct.set(VERTEX_COLOR, 4, XD_UBYTE);
	VertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);

	// Set refresh rate
	setRefreshRate(60);

	// Setup viewport
	Vector2i size = Window::getSize();
	s_graphicsContext.resizeViewport(size.x, size.y);
	Window::s_graphicsContext = &s_graphicsContext;

	// Init graphics
	glGenVertexArrays(1, &s_vao);
	glBindVertexArray(s_vao);
	glGenBuffers(1, &s_vbo);
	glGenBuffers(1, &s_ibo);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable alpha test // Optimization?
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LEQUAL, 0.0f);

	glPointSize(4);

	string vertexShader =
		"\n"
		"in vec2 in_position;\n"
		"in vec2 in_texCoord;\n"
		"in vec4 in_vertexColor;\n"
		"\n"
		"out vec2 texCoord;\n"
		"out vec4 vertexColor;\n"
		"\n"
		"uniform mat4 u_modelViewProj;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(in_position, 0.0, 1.0) * u_modelViewProj;\n"
		"	texCoord = in_texCoord;\n"
		"	vertexColor = in_vertexColor;\n"
		"}\n";

	string fragmentShader =
		"\n"
		"in vec2 texCoord;\n"
		"in vec4 vertexColor;\n"
		"\n"
		"out vec4 out_fragColor;\n"
		"\n"
		"uniform sampler2D u_texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	out_fragColor = texture2D(u_texture, texCoord) * vertexColor;\n"
		"}";

	s_defaultShader = ShaderPtr(new Shader(vertexShader, fragmentShader));
	s_defaultTexture = Texture2DPtr(new Texture2D(1, 1));
}

ShaderPtr Graphics::s_defaultShader = 0;
Texture2DPtr Graphics::s_defaultTexture = 0;
GLuint Graphics::s_vao = 0;
GLuint Graphics::s_vbo = 0;
GLuint Graphics::s_ibo = 0;

void Graphics::createContext()
{
	// Create Graphics rendering context
	s_context = wglCreateContext(Window::s_deviceContext);

	// Make context current
	wglMakeCurrent(Window::s_deviceContext, s_context);
}

void Graphics::destroyContext()
{
	glDeleteBuffers(1, &s_vbo);
	glDeleteVertexArrays(1, &s_vao);

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
	SwapBuffers(Window::s_deviceContext);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Vsync
void Graphics::enableVsync()
{
	if (wglSwapIntervalEXT) {
		wglSwapIntervalEXT(1);
	}
}

void Graphics::disableVsync()
{
	if (wglSwapIntervalEXT) {
		wglSwapIntervalEXT(0);
	}
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

END_XD_NAMESPACE