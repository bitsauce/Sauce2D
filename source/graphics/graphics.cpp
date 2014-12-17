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
float Graphics::s_currentOrtho[6];
int Graphics::s_currentViewport[4];
	
float Graphics::s_framesPerSecond = 0.0f;
int Graphics::s_refreshRate;
float Graphics::s_timeStep;

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

/*
Shader* Graphics::CreateShader(const string &vertFilePath, const string &fragFilePath)
{
	return s_this->createShader(vertFilePath, fragFilePath);
}

VertexBufferObject *Graphics::CreateVertexBufferObject(const VertexBuffer &buffer)
{
	return s_this->createVertexBufferObject(buffer);
}

RenderTarget2D *Graphics::CreateFrameBufferObject()
{
	return s_this->createFrameBufferObject();
}

bool Graphics::IsSupported(Feature feature)
{
	return s_this->isSupported(feature);
}*/

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
	setOrthoProjection(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);
	setViewport(Recti(0, 0, size.x, size.y));

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

void Graphics::setViewport(const Recti &rect)
{
	// Set viewport
	glViewport(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

	// Store viewport
	s_currentViewport[0] = rect.getX();
	s_currentViewport[1] = rect.getY();
	s_currentViewport[2] = rect.getWidth();
	s_currentViewport[3] = rect.getHeight();
}

void Graphics::getViewport(int &x, int &y, int &w, int &h)
{
	x = s_currentViewport[0];
	y = s_currentViewport[1];
	w = s_currentViewport[2];
	h = s_currentViewport[3];
}

void Graphics::setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)
{
	// Set orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(l, r, b, t, n, f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Store orthographic projection
	s_currentOrtho[0] = l;
	s_currentOrtho[1] = r;
	s_currentOrtho[2] = b;
	s_currentOrtho[3] = t;
	s_currentOrtho[4] = n;
	s_currentOrtho[5] = f;
}

void Graphics::getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f)
{
	// Get orthographic projection
	l = s_currentOrtho[0];
	r = s_currentOrtho[1];
	b = s_currentOrtho[2];
	t = s_currentOrtho[3];
	n = s_currentOrtho[4];
	f = s_currentOrtho[5];
}

const int INT_SIZE = sizeof(GLint);
const int FLOAT_SIZE = sizeof(GLfloat);
const int VERTEX_SIZE = sizeof(Vertex);

GLenum toGLBlend(const Batch::BlendFunc value)
{
	switch(value) {
	case Batch::BLEND_ZERO:				return GL_ZERO;
	case Batch::BLEND_ONE:					return GL_ONE;
	case Batch::BLEND_SRC_COLOR:			return GL_SRC_COLOR;
	case Batch::BLEND_ONE_MINUS_SRC_COLOR:	return GL_ONE_MINUS_SRC_COLOR;
	case Batch::BLEND_DST_COLOR:			return GL_DST_COLOR;
	case Batch::BLEND_ONE_MINUS_DST_COLOR:	return GL_ONE_MINUS_DST_COLOR;
	case Batch::BLEND_SRC_ALPHA:			return GL_SRC_ALPHA;
	case Batch::BLEND_ONE_MINUS_SRC_ALPHA:	return GL_ONE_MINUS_SRC_ALPHA;
	case Batch::BLEND_DST_ALPHA:			return GL_DST_ALPHA;
	case Batch::BLEND_ONE_MINUS_DST_ALPHA:	return GL_ONE_MINUS_DST_ALPHA;
	case Batch::BLEND_SRC_ALPHA_SATURATE:	return GL_SRC_ALPHA_SATURATE;
	}
	return GL_ZERO;
}

GLenum toGLPrimitive(const Batch::PrimitiveType value)
{
	switch(value) {
	case Batch::PRIMITIVE_POINTS:		return GL_POINTS;
	case Batch::PRIMITIVE_LINES:		return GL_LINES;
	case Batch::PRIMITIVE_TRIANGLES:	return GL_TRIANGLES;
	}
	return GL_TRIANGLES;
}

GLenum toGLType(const DataType value)
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

void Graphics::renderBatch(const Batch &batch)
{
	// Get buffers
	vector<Batch::VertexBufferState> buffers = batch.m_buffers;
	for(vector<Batch::VertexBufferState>::iterator itr = buffers.begin(); itr != buffers.end(); ++itr)
	{
		Batch::State &state = (*itr).state;
		
		glLoadMatrixf(state.projMat.getTranspose());

		if(state.shader)
		{
			// Enable shader
			ShaderPtr shader = state.shader;
			glUseProgram(shader->m_id);
			GLuint target = 0;

			// Set all uniforms
			for(map<string, Shader::Uniform*>::iterator itr = shader->m_uniforms.begin(); itr != shader->m_uniforms.end(); ++itr)
			{
				const Shader::Uniform *uniform = itr->second;
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
			if(state.texture)
			{
				glBindTexture(GL_TEXTURE_2D, state.texture->m_id);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			// Set blend func
			//glBlendFuncSeparate(state->srcBlendColor, state->dstBlendColor, state->srcBlendAlpha, state->dstBlendAlpha);
			glBlendFunc(toGLBlend(state.srcBlendFunc), toGLBlend(state.dstBlendFunc));
		}
		
		const VertexBuffer *buffer = (*itr).buffer.get();
		VertexFormat fmt = buffer->getVertexFormat();
		if(buffer->getBufferType() == VertexBuffer::RAW_BUFFER)
		{
			// Get vertices and vertex data
			char *vertexData = buffer->getVertexData();
			uint *indexData = buffer->getIndexData();
			
			// Set array pointers
			int stride = fmt.getVertexSizeInBytes();
			for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
			{
				VertexAttribute attrib = VertexAttribute(i);
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
			glDrawElements(toGLPrimitive(state.primitive), buffer->getIndexCount(), GL_UNSIGNED_INT, indexData);
		}
		else
		{
			// Bind vertices and indices array
			glBindBuffer(GL_ARRAY_BUFFER_ARB, buffer->getVBO()->m_vboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->getVBO()->m_iboId);
			
			// Set array pointers
			int stride = fmt.getVertexSizeInBytes();
			for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
			{
				VertexAttribute attrib = VertexAttribute(i);
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
			glDrawElements(toGLPrimitive(state.primitive), buffer->getIndexCount(), GL_UNSIGNED_INT, 0);

			// Reset vbo buffers
			glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
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