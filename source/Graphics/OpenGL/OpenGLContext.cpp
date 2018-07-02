#include <Sauce/Graphics.h>
#include <Sauce/Graphics/OpenGL/OpenGLRenderTarget.h>
#include <Sauce/Graphics/OpenGL/OpenGLTexture.h>
#include <Sauce/Graphics/OpenGL/OpenGLShader.h>

BEGIN_SAUCE_NAMESPACE

#ifdef SAUCE_DEBUG
void checkError(const string &info)
{
	GLenum error;
	if((error = glGetError()) != GL_NO_ERROR)
	{
		string errorType = "";
		switch(error)
		{
			case GL_INVALID_ENUM: errorType = "GL_INVALID_ENUM"; break;
			case GL_INVALID_VALUE: errorType = "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: errorType = "GL_INVALID_OPERATION"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: errorType = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_OUT_OF_MEMORY: errorType = "GL_OUT_OF_MEMORY"; break;
			case GL_STACK_UNDERFLOW: errorType = "GL_STACK_UNDERFLOW"; break;
			case GL_STACK_OVERFLOW: errorType = "GL_STACK_OVERFLOW"; break;
			default: errorType = "Unknown GL error"; break;
		}
		THROW("glGetError() returned 0x%X (%s) from function %s", error, errorType.c_str(), info.c_str());
	}
}

#define GL_CHECK_ERROR(info) checkError(#info)
#else
#define GL_CHECK_ERROR(info)
#endif

// Global GL objects for easy rendering
GLuint OpenGLContext::s_vao = 0;
GLuint OpenGLContext::s_vbo = 0;
GLuint OpenGLContext::s_ibo = 0;

OpenGLContext::OpenGLContext()
{
}

OpenGLContext::~OpenGLContext()
{
	glDeleteBuffers(1, &s_vbo);
	glDeleteVertexArrays(1, &s_vao);
	SDL_GL_DeleteContext(m_context);
}

void OpenGLContext::enable(const Capability cap)
{
	switch(cap) {
		case BLEND: glEnable(GL_BLEND); break;
		case DEPTH_TEST: glEnable(GL_DEPTH_TEST); break;
		case FACE_CULLING: glEnable(GL_CULL_FACE); break;
		case LINE_SMOOTH: glEnable(GL_LINE_SMOOTH); break;
		case POLYGON_SMOOTH: glEnable(GL_POLYGON_SMOOTH); break;
		case MULTISAMPLE: glEnable(GL_MULTISAMPLE); break;
		case TEXTURE_1D: glEnable(GL_TEXTURE_1D); break;
		case TEXTURE_2D: glEnable(GL_TEXTURE_2D); break;
		case TEXTURE_3D: glEnable(GL_TEXTURE_3D); break;
		case VSYNC:
			SDL_GL_SetSwapInterval(1);
			break;
		case WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
	}
}

void OpenGLContext::disable(const Capability cap)
{
	switch(cap) {
		case BLEND: glDisable(GL_BLEND); break;
		case DEPTH_TEST: glDisable(GL_DEPTH_TEST); break;
		case FACE_CULLING: glDisable(GL_CULL_FACE); break;
		case LINE_SMOOTH: glDisable(GL_LINE_SMOOTH); break;
		case POLYGON_SMOOTH: glDisable(GL_POLYGON_SMOOTH); break;
		case MULTISAMPLE: glDisable(GL_MULTISAMPLE); break;
		case TEXTURE_1D: glDisable(GL_TEXTURE_1D); break;
		case TEXTURE_2D: glDisable(GL_TEXTURE_2D); break;
		case TEXTURE_3D: glDisable(GL_TEXTURE_3D); break;
		case VSYNC:
			SDL_GL_SetSwapInterval(0);
			break;
		case WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
	}
}

bool OpenGLContext::isEnabled(const Capability cap)
{
	return (bool)glIsEnabled(cap);
}

void OpenGLContext::setPointSize(const float pointSize)
{
	glPointSize(pointSize);
}

void OpenGLContext::setLineWidth(const float lineWidth)
{
	glLineWidth(lineWidth);
}

void OpenGLContext::clear(const uint mask, const Color &fillColor)
{
	if(mask & COLOR_BUFFER) glClearColor(fillColor.getR() / 255.0f, fillColor.getG() / 255.0f, fillColor.getB() / 255.0f, fillColor.getA() / 255.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(fillColor.getR() / 255.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(fillColor.getR() / 255.0f);
	glClear(mask);
	if(mask & COLOR_BUFFER) glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(0.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(0.0f);
}

void OpenGLContext::enableScissor(const int x, const int y, const int w, const int h)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
}

void OpenGLContext::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void OpenGLContext::saveScreenshot(string path)
{
	// Get frame buffer data
	uchar *data = new uchar[m_currentState->width * m_currentState->height * 4];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, m_currentState->width, m_currentState->height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadBuffer(GL_BACK);

	// NOTE: This function is not tested!
	Pixmap pixmap(m_currentState->width, m_currentState->height, data);
	pixmap.flipY();
	pixmap.exportToFile(path);

	delete[] data;
}

// Orthographic projection
void OpenGLContext::setViewportSize(const uint w, const uint h)
{
	// Set viewport
	glViewport(0, 0, w, h);
}

Matrix4 OpenGLContext::createOrtographicMatrix(const float l, const float r, const float t, const float b, const float n, const float f) const
{
	/*
	// TODO: Add flipY
	float l = 0.0f,
		r = (float) m_currentState->width,
		b = flipY ? 0.0f : (float) m_currentState->height,
		t = flipY ? (float) m_currentState->height : 0.0f,
		n = -1.0f,
		f = 1.0f;*/

	// Returns an ortographic projection matrix (typically for 2D rendering)
	Matrix4 mat(
		2.0f / (r - l), 0.0f,            0.0f,           -((r + l) / (r - l)),
		0.0f,           2.0f / (t - b),  0.0f,           -((t + b) / (t - b)),
		0.0f,           0.0f,           -2.0f / (f - n), -((f + n) / (f - n)),
		0.0f,           0.0f,            0.0f,            1.0f);
	return mat;
}

Matrix4 OpenGLContext::createPerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar) const
{
	// Returns a perspective matrix
	const float s = tanf(math::degToRad(fov / 2.0f));
	Matrix4 mat(
		1.0f / (s * aspectRatio),    0.0f,      0.0f,                             0.0f,
		0.0f,                        1.0f / s,  0.0f,                             0.0f,
		0.0f,                        0.0f,     -(zFar + zNear) / (zFar - zNear), -(2 * zFar * zNear) / (zFar - zNear) ,
		0.0f,                        0.0f,     -1.0f,                             0.0f);
	return mat;
}

Matrix4 OpenGLContext::createLookAtMatrix(const Vector3F &position, const Vector3F &fwd) const
{
	const Vector3F worldUp(0.0f, 1.0f, 0.0f);
	const Vector3F right = math::normalize(math::cross(worldUp, fwd));
	const Vector3F up = math::cross(fwd, right);
	Matrix4 cameraMatrix(
		right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		fwd.x, fwd.y, fwd.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 cameraTranslate(
		1, 0, 0, -position.x,
		0, 1, 0, -position.y,
		0, 0, 1, -position.z,
		0.0f, 0.0f, 0.0f, 1.0f);
	return cameraMatrix * cameraTranslate;
}

Window *OpenGLContext::createWindow(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags)
{
	// Request opengl 3.1 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_window = new Window(this, title, x, y, w, h, SDL_WINDOW_OPENGL | flags);

	// Create GL context
	m_context = SDL_GL_CreateContext(m_window->getSDLHandle());

	// Initialize GL3W
	if(gl3wInit() != 0)
	{
		THROW("GL3W did not initialize!");
	}

	// Print GPU info
	LOG("** Using GPU: %s (OpenGL %s) **", glGetString(GL_VENDOR), glGetString(GL_VERSION));

	// Check OpenGL 3.1 support
	if(!gl3wIsSupported(3, 1))
	{
		THROW("OpenGL 3.1 not supported\n");
	}

	// Setup graphics context
	Vector2I size;
	m_window->getSize(&size.x, &size.y);
	setSize(size.x, size.y);

	// We default to an ortographic projection where top-left is (0, 0) and bottom-right is (w, h)
	setProjectionMatrix(createOrtographicMatrix(0, size.x, 0, size.y));

	// Init graphics
	glGenVertexArrays(1, &s_vao);
	glBindVertexArray(s_vao);
	glGenBuffers(1, &s_vbo);
	glGenBuffers(1, &s_ibo);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set smooth lines
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Set some gl state variables
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glPointSize(4);

	// Create passthrough shader
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

	s_defaultShader = shared_ptr<Shader>(new OpenGLShader(vertexShader, fragmentShader, ""));

	// Create blank texture
	uchar pixel[4];
	pixel[0] = pixel[1] = pixel[2] = pixel[3] = 255;
	s_defaultTexture = shared_ptr<Texture2D>(GraphicsContext::createTexture(1, 1, pixel));

	return m_window;
}

void OpenGLContext::setupContext()
{
	// Set blend func
	glBlendFuncSeparate(m_currentState->blendState.m_src, m_currentState->blendState.m_dst, m_currentState->blendState.m_alphaSrc, m_currentState->blendState.m_alphaDst);
	GL_CHECK_ERROR(glBlendFuncSeparate);

	shared_ptr<Shader> shader = m_currentState->shader;
	if(!shader)
	{
		shader = s_defaultShader;
		shader->setSampler2D("u_Texture", m_currentState->texture == 0 ? s_defaultTexture : m_currentState->texture);
	}

	OpenGLShader *glShader = dynamic_cast<OpenGLShader*>(shader.get());

	// Enable shader
	glUseProgram(glShader->m_id);
	GL_CHECK_ERROR(glUseProgram);

	// Set projection matrix
	Matrix4 modelViewProjection = m_currentState->projectionMatrix * m_currentState->transformationMatrixStack.top();
	shader->setUniformMatrix4f("u_ModelViewProj", modelViewProjection.get());

	GLuint target = 0;

	// Set all uniforms
	for(map<string, OpenGLShader::Uniform*>::iterator itr = glShader->m_uniforms.begin(); itr != glShader->m_uniforms.end(); ++itr)
	{
		const OpenGLShader::Uniform *uniform = itr->second;
		switch(uniform->type)
		{
			case GL_INT: case GL_BOOL: glUniform1iv(uniform->loc, uniform->count, (const GLint*) uniform->data); break;
			case GL_INT_VEC2: case GL_BOOL_VEC2: glUniform2i(uniform->loc, ((GLint*) uniform->data)[0], ((GLint*) uniform->data)[1]); break;
			case GL_INT_VEC3: case GL_BOOL_VEC3: glUniform3i(uniform->loc, ((GLint*) uniform->data)[0], ((GLint*) uniform->data)[1], ((GLint*) uniform->data)[2]); break;
			case GL_INT_VEC4: case GL_BOOL_VEC4: glUniform4i(uniform->loc, ((GLint*) uniform->data)[0], ((GLint*) uniform->data)[1], ((GLint*) uniform->data)[2], ((GLint*) uniform->data)[3]); break;

			case GL_UNSIGNED_INT: glUniform1ui(uniform->loc, ((GLuint*) uniform->data)[0]); break;
			case GL_UNSIGNED_INT_VEC2: glUniform2ui(uniform->loc, ((GLuint*) uniform->data)[0], ((GLuint*) uniform->data)[1]); break;
			case GL_UNSIGNED_INT_VEC3: glUniform3ui(uniform->loc, ((GLuint*) uniform->data)[0], ((GLuint*) uniform->data)[1], ((GLuint*) uniform->data)[2]); break;
			case GL_UNSIGNED_INT_VEC4: glUniform4ui(uniform->loc, ((GLuint*) uniform->data)[0], ((GLuint*) uniform->data)[1], ((GLuint*) uniform->data)[2], ((GLuint*) uniform->data)[3]); break;

			case GL_FLOAT: glUniform1f(uniform->loc, ((GLfloat*) uniform->data)[0]); break;
			case GL_FLOAT_VEC2: glUniform2fv(uniform->loc, uniform->count, (const GLfloat*) uniform->data); break;
			case GL_FLOAT_VEC3: glUniform3f(uniform->loc, ((GLfloat*) uniform->data)[0], ((GLfloat*) uniform->data)[1], ((GLfloat*) uniform->data)[2]); break;
			case GL_FLOAT_VEC4: glUniform4fv(uniform->loc, uniform->count, (const GLfloat*) uniform->data); break;

			case GL_FLOAT_MAT4: glUniformMatrix4fv(uniform->loc, 1, GL_FALSE, (GLfloat*) uniform->data); break;

			case GL_UNSIGNED_INT_SAMPLER_2D:
			case GL_INT_SAMPLER_2D:
			case GL_SAMPLER_2D:
			{
				glActiveTexture(GL_TEXTURE0 + target);
				glBindTexture(GL_TEXTURE_2D, ((GLuint*) uniform->data)[0]);
				glUniform1i(uniform->loc, target++);
			}
			break;
		}
		GL_CHECK_ERROR(glUniform1i);
	}
}

void OpenGLContext::drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount)
{
	// If there are no vertices to draw, do nothing
	if(vertexCount == 0 || indexCount == 0) return;

	setupContext();

	// Get vertices and vertex data
	VertexFormat fmt = vertices->getFormat();
	int vertexSizeInBytes = fmt.getVertexSizeInBytes();
	char *vertexData = new char[vertexCount * vertexSizeInBytes];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * vertexSizeInBytes);
	}

	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo); GL_CHECK_ERROR(glBindBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSizeInBytes, vertexData, GL_DYNAMIC_DRAW); GL_CHECK_ERROR(glBufferData);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo); GL_CHECK_ERROR(glBindBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_DYNAMIC_DRAW); GL_CHECK_ERROR(glBufferData);

	// Set array pointers
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch(attrib)
		{
			case VERTEX_POSITION:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(0); GL_CHECK_ERROR(glEnableVertexAttribArray);
					glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib)); GL_CHECK_ERROR(glVertexAttribPointer);
				}
				else
				{
					glDisableVertexAttribArray(0); GL_CHECK_ERROR(glDisableVertexAttribArray);
				}
				break;

			case VERTEX_COLOR:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(1); GL_CHECK_ERROR(glEnableVertexAttribArray);
					glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib)); GL_CHECK_ERROR(glVertexAttribPointer);
				}
				else
				{
					glDisableVertexAttribArray(1); GL_CHECK_ERROR(glDisableVertexAttribArray);
				}
				break;

			case VERTEX_TEX_COORD:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(2); GL_CHECK_ERROR(glEnableVertexAttribArray);
					glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib)); GL_CHECK_ERROR(glVertexAttribPointer);
				}
				else
				{
					glDisableVertexAttribArray(2); GL_CHECK_ERROR(glDisableVertexAttribArray);
				}
				break;
		}
	}

	// Draw primitives
	glDrawElements(type, indexCount, GL_UNSIGNED_INT, 0); GL_CHECK_ERROR(glDrawElements);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0); GL_CHECK_ERROR(glBindBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GL_CHECK_ERROR(glBindBuffer);

	// Release vertex data
	delete[] vertexData;
}

void OpenGLContext::drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo)
{
	// If one of the buffers are empty, do nothing
	if(vbo->getSize() == 0 || ibo->getSize() == 0) return; 

	setupContext();

	// Bind vertices and indices array
	glBindBuffer(GL_ARRAY_BUFFER, vbo->m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->m_id);

	// Set array pointers
	VertexFormat fmt = vbo->getVertexFormat();
	int stride = fmt.getVertexSizeInBytes();
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch(attrib)
		{
			case VERTEX_POSITION:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(0);
				}
				break;

			case VERTEX_COLOR:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(1);
				}
				break;

			case VERTEX_TEX_COORD:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(2);
				}
				break;
		}
	}

	// Draw vbo
	glDrawElements(type, ibo->getSize(), GL_UNSIGNED_INT, 0);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GL_CHECK_ERROR(glBindBuffer);
}

void OpenGLContext::drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount)
{
	// If there are no vertices to draw, do nothing
	if(vertexCount == 0) return;

	setupContext();

	// Get vertices and vertex data
	VertexFormat fmt = vertices->getFormat();
	int vertexSizeInBytes = fmt.getVertexSizeInBytes();
	char *vertexData = new char[vertexCount * vertexSizeInBytes];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * vertexSizeInBytes);
	}

	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSizeInBytes, vertexData, GL_DYNAMIC_DRAW);

	// Set array pointers
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch(attrib)
		{
			case VERTEX_POSITION:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(0);
				}
				break;

			case VERTEX_COLOR:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(1);
				}
				break;

			case VERTEX_TEX_COORD:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(2);
				}
				break;
		}
	}

	// Draw primitives
	glDrawArrays(type, 0, vertexCount);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GL_CHECK_ERROR(glBindBuffer);

	// Release vertex data
	delete[] vertexData;
}

void OpenGLContext::drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo)
{
	// If the buffer is empty, do nothing
	if(vbo->getSize() == 0) return;

	setupContext();

	// Bind vertices and indices array
	glBindBuffer(GL_ARRAY_BUFFER, vbo->m_id);

	// Set array pointers
	VertexFormat fmt = vbo->getVertexFormat();
	int stride = fmt.getVertexSizeInBytes();
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch(attrib)
		{
			case VERTEX_POSITION:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(0);
				}
				break;

			case VERTEX_COLOR:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(1);
				}
				break;

			case VERTEX_TEX_COORD:
				if(fmt.isAttributeEnabled(attrib))
				{
					glEnableVertexAttribArray(2);
					glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*) fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableVertexAttribArray(2);
				}
				break;
		}
	}

	// Draw vbo
	glDrawArrays(type, 0, vbo->getSize());

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GL_CHECK_ERROR(glBindBuffer);
}

RenderTarget2D *OpenGLContext::createRenderTarget(const uint width, const uint height, const uint targetCount, const PixelFormat &format)
{
	return static_cast<RenderTarget2D*>(new OpenGLRenderTarget2D(this, width, height, targetCount, format));
}

Texture2D *OpenGLContext::createTexture(const Pixmap &pixmap)
{
	return static_cast<Texture2D*>(new OpenGLTexture2D(pixmap));
}

Shader *OpenGLContext::createShader(const string &vertexSource, const string &fragmentSource, const string &geometrySource)
{
	return static_cast<Shader*>(new OpenGLShader(vertexSource, fragmentSource, geometrySource));
}

END_SAUCE_NAMESPACE
