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

double Graphics::s_framesPerSecond = 0.0;
GraphicsContext Graphics::s_graphicsContext;
ShaderPtr Graphics::s_defaultShader = 0;
Texture2DPtr Graphics::s_defaultTexture = 0;
GLuint Graphics::s_vao = 0;
GLuint Graphics::s_vbo = 0;
GLuint Graphics::s_ibo = 0;
int Graphics::s_vsync = 0;

double Graphics::getFPS()
{
	return s_framesPerSecond;
}

void Graphics::init()
{
	// Initialize the GLFW library
	if(gl3wInit() != 0) {
		assert("GLEW did not initialize!");
	}
	
	// Print GPU info
	LOG("** Using GPU: %s (OpenGL %s) **", glGetString(GL_VENDOR), glGetString(GL_VERSION));

	// Check OpenGL 3.1 support
	if (!gl3wIsSupported(3, 1)) {
		assert("OpenGL 3.1 not supported\n");
	}

	// Setup default vertex format
	VertexFormat::s_vct.set(VERTEX_POSITION, 2);
	VertexFormat::s_vct.set(VERTEX_COLOR, 4, XD_UBYTE);
	VertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);

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
		"in vec2 in_Position;\n"
		"in vec2 in_TexCoord;\n"
		"in vec4 in_VertexColor;\n"
		"\n"
		"out vec2 v_TexCoord;\n"
		"out vec4 v_VertexColor;\n"
		"\n"
		"uniform mat4 u_ModelViewProj;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(in_Position, 0.0, 1.0) * u_ModelViewProj;\n"
		"	v_TexCoord = in_TexCoord;\n"
		"	v_VertexColor = in_VertexColor;\n"
		"}\n";

	string fragmentShader =
		"\n"
		"in vec2 v_TexCoord;\n"
		"in vec4 v_VertexColor;\n"
		"\n"
		"out vec4 out_FragColor;\n"
		"\n"
		"uniform sampler2D u_Texture;"
		"\n"
		"void main()\n"
		"{\n"
		"	out_FragColor = texture(u_Texture, v_TexCoord) * v_VertexColor;\n"
		"}\n";

	s_defaultShader = ShaderPtr(new Shader(vertexShader, fragmentShader));

	uchar pixel[4];
	pixel[0] = pixel[1] = pixel[2] = pixel[3] = 255;
	s_defaultTexture = Texture2DPtr(new Texture2D(1, 1, pixel));
}

void Graphics::clear()
{
	glDeleteBuffers(1, &s_vbo);
	glDeleteVertexArrays(1, &s_vao);
}

void Graphics::swapBuffers()
{
	glfwSwapBuffers(Window::s_window);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Vsync
void Graphics::setVsync(const int mode)
{
	glfwSwapInterval(mode);
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

END_XD_NAMESPACE