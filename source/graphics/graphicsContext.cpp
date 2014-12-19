#include <x2d/graphics.h>

namespace xd {

GraphicsContext::GraphicsContext() :
	m_width(0),
	m_height(0),
	m_renderTarget(nullptr)
{
}

void GraphicsContext::enable(const Capability cap)
{
	glEnable(cap);
}

void GraphicsContext::disable(const Capability cap)
{
	glDisable(cap);
}

void GraphicsContext::setRenderTarget(RenderTarget2D *renderTarget)
{
	// Setup viewport and projection
	if(m_renderTarget != renderTarget)
	{
		if(renderTarget)
		{
			// Bind render target
			m_renderTarget = renderTarget;
			m_renderTarget->bind();

			// Resize viewport
			resizeViewport(m_renderTarget->m_width, m_renderTarget->m_height);
		}
		else
		{
			// Unbind render target
			m_renderTarget->unbind();
			m_renderTarget = nullptr;

			// Reset viewport
			resizeViewport(XWindow::getSize().x, XWindow::getSize().y);
		}
	}
}
	
// Orthographic projection
void GraphicsContext::resizeViewport(const uint w, const uint h)
{
	// Set size
	m_width = w;
	m_height = h;

	// Set orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set viewport
	glViewport(0, 0, m_width, m_height);
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

void GraphicsContext::renderBatch(const Batch &batch)
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

}