//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "glgraphics.h"
#include "gltexture.h"
#include "glshader.h"
#include "common/window.h"

#include <x2d/x2d.h>

GLenum toGLBufferType(const XVertexBuffer::BufferType value)
{
	switch(value) {
	case XVertexBuffer::DYNAMIC_BUFFER:		return GL_DYNAMIC_DRAW;
	case XVertexBuffer::STATIC_BUFFER:		return GL_STATIC_DRAW;
	}
	return GL_DYNAMIC_DRAW;
}

class GLvertexbuffer : public XVertexBufferObject
{
	friend class OpenGL;
public:
	GLvertexbuffer(const XVertexBuffer &buffer) :
		m_vertexFormat(buffer.getVertexFormat())
	{
		// Generate buffers
		glGenBuffers(1, &m_vboId);
		glGenBuffers(1, &m_iboId);

		// Upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(GL_ARRAY_BUFFER, buffer.getVertexCount() * m_vertexFormat.getVertexSizeInBytes(), buffer.getVertexData(), toGLBufferType(buffer.getBufferType()));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.getIndexCount() * sizeof(uint), buffer.getIndexData(), toGLBufferType(buffer.getBufferType()));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~GLvertexbuffer()
	{
		glDeleteBuffers(1, &m_vboId);
		glDeleteBuffers(1, &m_iboId);
	}

	void update(const int offset, const char *data, const int count)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_vertexFormat.getVertexSizeInBytes(), count * m_vertexFormat.getVertexSizeInBytes(), data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

private:
	GLuint m_vboId;
	GLuint m_iboId;
	XVertexFormat m_vertexFormat;
};

class GLframebufferobject : public XFrameBufferObject
{
public:
	GLframebufferobject(OpenGL *gl) :
		gl(gl)
	{
		glGenFramebuffers(1, &m_id);
	}

	void bind(XTexture *texture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((GLtexture*)texture)->m_id, 0);

		gl->getOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
		gl->getViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		
		int w = texture->getWidth(), h = texture->getHeight();
		gl->setOrthoProjection(0.0f, (float)w, (float)h, 0.0f, m_ortho[4], m_ortho[5]);
		gl->setViewport(Recti(0, 0, w, h));
	}

	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		gl->setOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
		gl->setViewport(Recti(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]));
	}

private:
	GLuint m_id;
	float m_ortho[6];
	int m_viewport[4];
	OpenGL *gl;
};

void OpenGL::init(Window *window)
{
	// Init glew
	//glewExperimental = true;
	if(glewInit() != GLEW_OK)
		assert("GLEW did not initialize!");

	// TODO: Do better extention handling. LOG will crash because xdEngine isn't initialized
	// Check if non-power of two textures are supported
	//if(!GLEW_ARB_texture_non_power_of_two)
	//	LOG("WARNING: NPOT is not supported on this card!");

	// Check if FBOs are supported
	//if(!GLEW_EXT_framebuffer_object)
	//	LOG("WARNING: FBO is not supported on this card!");


	// Check if PBOs are supported
	//if(!GLEW_EXT_pixel_buffer_object)
	//	LOG("WARNING: PBO is not supported on this card!");

	// Check if (changing) v-sync (state) is supported
	//if(!WGLEW_EXT_swap_control)
	//	LOG("WARNING: VSYNC is not supported on this card!");

	Vector2i size = window->getSize();
	setOrthoProjection(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);
	setViewport(Recti(0, 0, size.x, size.y));

	// Init OpenGL
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable alpha test // Optimization?
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LEQUAL, 0.0f);

	// Set OpenGL hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(4);
}

OpenGL::~OpenGL()
{
}

void OpenGL::createContext()
{
	// Create OpenGL rendering context
	m_context = wglCreateContext(m_deviceContext);

	// Make context current
	wglMakeCurrent(m_deviceContext, m_context);
}

void OpenGL::destroyContext()
{
	if(m_context)
	{
		// Make the rendering context not current
		wglMakeCurrent(NULL, NULL);

		// Delete the OpenGL rendering context
		wglDeleteContext(m_context);

		// Set to null
		m_context = NULL;
	}
}

void OpenGL::swapBuffers()
{
	SwapBuffers(m_deviceContext);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::setViewport(const Recti &rect)
{
	// Set viewport
	glViewport(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

	// Store viewport
	m_currentViewport[0] = rect.getX();
	m_currentViewport[1] = rect.getY();
	m_currentViewport[2] = rect.getWidth();
	m_currentViewport[3] = rect.getHeight();
}

void OpenGL::getViewport(int &x, int &y, int &w, int &h)
{
	x = m_currentViewport[0];
	y = m_currentViewport[1];
	w = m_currentViewport[2];
	h = m_currentViewport[3];
}

void OpenGL::setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)
{
	// Set orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(l, r, b, t, n, f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Store orthographic projection
	m_currentOrtho[0] = l;
	m_currentOrtho[1] = r;
	m_currentOrtho[2] = b;
	m_currentOrtho[3] = t;
	m_currentOrtho[4] = n;
	m_currentOrtho[5] = f;
}

void OpenGL::getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f)
{
	// Get orthographic projection
	l = m_currentOrtho[0];
	r = m_currentOrtho[1];
	b = m_currentOrtho[2];
	t = m_currentOrtho[3];
	n = m_currentOrtho[4];
	f = m_currentOrtho[5];
}

const int INT_SIZE = sizeof(GLint);
const int FLOAT_SIZE = sizeof(GLfloat);
const int VERTEX_SIZE = sizeof(XVertex);

GLenum toGLBlend(const XBatch::BlendFunc value)
{
	switch(value) {
	case XBatch::BLEND_ZERO:				return GL_ZERO;
	case XBatch::BLEND_ONE:					return GL_ONE;
	case XBatch::BLEND_SRC_COLOR:			return GL_SRC_COLOR;
	case XBatch::BLEND_ONE_MINUS_SRC_COLOR:	return GL_ONE_MINUS_SRC_COLOR;
	case XBatch::BLEND_DST_COLOR:			return GL_DST_COLOR;
	case XBatch::BLEND_ONE_MINUS_DST_COLOR:	return GL_ONE_MINUS_DST_COLOR;
	case XBatch::BLEND_SRC_ALPHA:			return GL_SRC_ALPHA;
	case XBatch::BLEND_ONE_MINUS_SRC_ALPHA:	return GL_ONE_MINUS_SRC_ALPHA;
	case XBatch::BLEND_DST_ALPHA:			return GL_DST_ALPHA;
	case XBatch::BLEND_ONE_MINUS_DST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
	case XBatch::BLEND_SRC_ALPHA_SATURATE:	return GL_SRC_ALPHA_SATURATE;
	}
	return GL_ZERO;
}

GLenum toGLPrimitive(const XBatch::PrimitiveType value)
{
	switch(value) {
	case XBatch::PRIMITIVE_POINTS:		return GL_POINTS;
	case XBatch::PRIMITIVE_LINES:		return GL_LINES;
	case XBatch::PRIMITIVE_TRIANGLES:	return GL_TRIANGLES;
	}
	return GL_TRIANGLES;
}

GLenum toGLType(const XDataType value)
{
	switch(value) {
	case XD_FLOAT:		return GL_FLOAT;
	case XD_UINT:		return GL_UNSIGNED_INT;
	case XD_INT:		return GL_INT;
	case XD_USHORT:		return GL_UNSIGNED_SHORT;
	case XD_SHORT:		return GL_SHORT;
	case XD_UBYTE:		return GL_UNSIGNED_BYTE;
	case XD_BYTE:		return GL_BYTE;
	}
	return GL_FLOAT;
}

void OpenGL::renderBatch(const XBatch &batch)
{
	// Get buffers
	StateVertexMap buffers = batch.m_buffers;
	Matrix4 mat = batch.m_projMatrix;
	glLoadMatrixf(mat.getTranspose());

	for(StateVertexMap::iterator itr = buffers.begin(); itr != buffers.end(); ++itr)
	{
		const XBatch::State &state = itr->first;
		if(state.shader)
		{
			// Enable shader
			GLshader *shader = (GLshader*)state.shader;
			glUseProgram(shader->m_id);
			GLuint target = 0;

			// Set all uniforms
			for(map<string, GLshader::Uniform*>::iterator itr = shader->m_uniforms.begin(); itr != shader->m_uniforms.end(); ++itr)
			{
				const GLshader::Uniform *uniform = itr->second;
				switch(uniform->type)
				{
				case GL_INT: glUniform1i(uniform->loc, ((GLint*)uniform->data)[0]); break;
				case GL_INT_VEC2: glUniform2i(uniform->loc, ((GLint*)uniform->data)[0], ((GLint*)uniform->data)[1]); break;
				case GL_INT_VEC3: glUniform3i(uniform->loc, ((GLint*)uniform->data)[0], ((GLint*)uniform->data)[1], ((GLint*)uniform->data)[2]); break;
				case GL_INT_VEC4: glUniform4i(uniform->loc, ((GLint*)uniform->data)[0], ((GLint*)uniform->data)[1], ((GLint*)uniform->data)[2], ((GLint*)uniform->data)[3]); break;
				case GL_FLOAT: glUniform1f(uniform->loc, ((GLfloat*)uniform->data)[0]); break;
				case GL_FLOAT_VEC2: glUniform2f(uniform->loc, ((GLfloat*)uniform->data)[0], ((GLfloat*)uniform->data)[1]); break;
				case GL_FLOAT_VEC3: glUniform3f(uniform->loc, ((GLfloat*)uniform->data)[0], ((GLfloat*)uniform->data)[1], ((GLfloat*)uniform->data)[2]); break;
				case GL_FLOAT_VEC4: glUniform4f(uniform->loc, ((GLfloat*)uniform->data)[0], ((GLfloat*)uniform->data)[1], ((GLfloat*)uniform->data)[2], ((GLfloat*)uniform->data)[3]); break;
				case GL_SAMPLER_2D:
					{
						glActiveTexture(GL_TEXTURE0+target);
						glBindTexture(GL_TEXTURE_2D, ((GLuint*)uniform->data)[0]);
						glUniform1i(uniform->loc, target++);
					}
					break;
				}
			}
		}
		else
		{
			// Disable shader
			glUseProgram(0);

			// Set texture unit 0
			glActiveTexture(GL_TEXTURE0);
			if(state.texture) {
				glBindTexture(GL_TEXTURE_2D, ((GLtexture*)state.texture)->m_id);
			}else{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			// Set blend func
			//glBlendFuncSeparate(state->srcBlendColor, state->dstBlendColor, state->srcBlendAlpha, state->dstBlendAlpha);
			glBlendFunc(toGLBlend(state.srcBlendFunc), toGLBlend(state.dstBlendFunc));
		}
		
		XVertexBuffer &buffer = itr->second;
		XVertexFormat fmt = buffer.getVertexFormat();
		if(buffer.getBufferType() == XVertexBuffer::RAW_BUFFER)
		{
			// Get vertices and vertex data
			char *vertexData = buffer.getVertexData();
			uint *indexData = buffer.getIndexData();
			
			// Set array pointers
			int stride = fmt.getVertexSizeInBytes();
			for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
			{
				XVertexAttribute attrib = XVertexAttribute(i);
				switch(attrib)
				{
					case VERTEX_POSITION:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_VERTEX_ARRAY);
							glVertexPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, vertexData + fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_VERTEX_ARRAY);
						}
						break;

					case VERTEX_COLOR:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_COLOR_ARRAY);
							glColorPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, vertexData + fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_COLOR_ARRAY);
						}
						break;

					case VERTEX_TEX_COORD:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glTexCoordPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, vertexData + fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						}
						break;
				}
			}

			// Draw batch
			glDrawElements(toGLPrimitive(state.primitive), buffer.getIndexCount(), GL_UNSIGNED_INT, indexData);
		}
		else
		{
			// Bind vertices and indices array
			glBindBuffer(GL_ARRAY_BUFFER_ARB, ((GLvertexbuffer*)buffer.getVBO())->m_vboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLvertexbuffer*)buffer.getVBO())->m_iboId);
			
			// Set array pointers
			int stride = fmt.getVertexSizeInBytes();
			for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
			{
				XVertexAttribute attrib = XVertexAttribute(i);
				switch(attrib)
				{
					case VERTEX_POSITION:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_VERTEX_ARRAY);
							glVertexPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, (void*)fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_VERTEX_ARRAY);
						}
						break;

					case VERTEX_COLOR:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_COLOR_ARRAY);
							glColorPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, (void*)fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_COLOR_ARRAY);
						}
						break;

					case VERTEX_TEX_COORD:
						if(fmt.isAttributeEnabled(attrib))
						{
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glTexCoordPointer(fmt.getElementCount(attrib), toGLType(fmt.getDataType(attrib)), stride, (void*)fmt.getAttributeOffset(attrib));
						}
						else
						{
							glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						}
						break;
				}
			}

			// Draw vbo
			glDrawElements(toGLPrimitive(state.primitive), buffer.getIndexCount(), GL_UNSIGNED_INT, 0);

			// Reset vbo buffers
			glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	glLoadIdentity();

	// Disable client state
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

XTexture *OpenGL::createTexture(const XPixmap &pixmap)
{
	return new GLtexture(pixmap);
}

XShader *OpenGL::createShader(const string &vertFilePath, const string &fragFilePath)
{
	return new GLshader(vertFilePath, fragFilePath);
}

XVertexBufferObject *OpenGL::createVertexBufferObject(const XVertexBuffer &buffer)
{
	return new GLvertexbuffer(buffer);
}

XFrameBufferObject *OpenGL::createFrameBufferObject()
{
	return new GLframebufferobject(this);
}

bool OpenGL::isSupported(Feature feature)
{
	switch(feature)
	{
	case VertexBufferObjects: return GLEW_ARB_vertex_buffer_object == GL_TRUE; break;
	case FrameBufferObjects: return GLEW_ARB_framebuffer_object == GL_TRUE; break;
	}
	return false;
}

// Vsync
void OpenGL::enableVsync()
{
	wglSwapIntervalEXT(1);
}

void OpenGL::disableVsync()
{
	wglSwapIntervalEXT(0);
}

// Wireframe
void OpenGL::enableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
	
void OpenGL::disableWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Alpha blending
void OpenGL::enableAlphaBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
	
void OpenGL::disableAlphaBlending()
{
	glDisable(GL_BLEND);
}