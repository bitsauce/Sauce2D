#include <x2d/graphics.h>

BEGIN_XD_NAMESPACE

GraphicsContext::GraphicsContext() :
	m_width(0),
	m_height(0),
	m_renderTarget(nullptr),
	m_shader(nullptr),
	m_texture(nullptr),
	m_blendState(BlendState::PRESET_ALPHA_BLEND)
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

void GraphicsContext::clear(const uint mask, const Color &fillColor)
{
	if(mask & COLOR_BUFFER) glClearColor(fillColor.r/255.0f, fillColor.g/255.0f, fillColor.b/255.0f, fillColor.a/255.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(fillColor.r/255.0f);
	if(mask & ACCUM_BUFFER) glClearAccum(fillColor.r/255.0f, fillColor.g/255.0f, fillColor.b/255.0f, fillColor.a/255.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(fillColor.r/255.0f);
	glClear(mask);
	if(mask & COLOR_BUFFER) glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(0.0f);
	if(mask & ACCUM_BUFFER) glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(0.0f);
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
			resizeViewport(Window::getSize().x, Window::getSize().y);
		}
	}
}

void GraphicsContext::setProjectionMatrix(const Matrix4 &projmat)
{
	while(!m_matrixStack.empty()) m_matrixStack.pop();
	m_matrixStack.push(projmat);
}

Matrix4 GraphicsContext::getProjectionMatrix() const
{
	if(m_matrixStack.empty()) return Matrix4();
	return m_matrixStack.top();
}

void GraphicsContext::pushMatrix(const Matrix4 &mat)
{
	if(m_matrixStack.empty()) m_matrixStack.push(mat);
	else m_matrixStack.push(m_matrixStack.top() * mat);
}

void GraphicsContext::popMatrix()
{
	if(m_matrixStack.empty()) return;
	m_matrixStack.pop();
}

void GraphicsContext::setTexture(const Texture2DPtr texture)
{
	m_texture = texture;
}

Texture2DPtr GraphicsContext::getTexture() const
{
	return m_texture;
}

void GraphicsContext::setShader(const ShaderPtr shader)
{
	m_shader = shader;
}

ShaderPtr GraphicsContext::getShader() const
{
	return m_shader;
}

void GraphicsContext::setBlendState(const BlendState &blendState)
{
	m_blendState = blendState;
}

BlendState GraphicsContext::getBlendState()
{
	return m_blendState;
}

#include <freeimage.h>
void GraphicsContext::saveScreenshot(string path)
{
	// Get frame buffer data
	uchar *data = new uchar[m_width*m_height*3];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, data);
	glReadBuffer(GL_BACK);

	FIBITMAP *bitmap = FreeImage_ConvertFromRawBits(data, m_width, m_height, m_width * 3, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
	util::toAbsoluteFilePath(path);
	FreeImage_Save(FIF_PNG, bitmap, path.c_str(), PNG_DEFAULT); // For now, let's just save everything as png

	delete[] data;
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

void GraphicsContext::setupContext()
{
	// Set projection matrix
	glLoadMatrixf((m_matrixStack.empty() ? Matrix4() : m_matrixStack.top()).getTranspose());

	// Set blend func
	glBlendFuncSeparate(m_blendState.m_src,  m_blendState.m_dst, m_blendState.m_alphaSrc, m_blendState.m_alphaDst);
	
	// Do we have a shader?
	if(m_shader)
	{
		// Enable shader
		glUseProgram(m_shader->m_id);
		GLuint target = 0;

		// Set all uniforms
		for(map<string, Shader::Uniform*>::iterator itr = m_shader->m_uniforms.begin(); itr != m_shader->m_uniforms.end(); ++itr)
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
		// Disable shaders
		glUseProgram(0);

		// Activate texture slot 0
		glActiveTexture(GL_TEXTURE0);

		// Bind texture
		glBindTexture(GL_TEXTURE_2D, (m_texture ? m_texture->m_id : 0));
	}
}

void GraphicsContext::drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount)
{
	setupContext();

	// Get vertices and vertex data
	VertexFormat fmt = vertices->getFormat();
	char *vertexData = new char[vertexCount * fmt.getVertexSizeInBytes()];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * fmt.getVertexSizeInBytes());
	}
			
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
					glVertexPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
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
					glColorPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
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
					glTexCoordPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				break;
		}
	}

	glDrawElements(type, indexCount, GL_UNSIGNED_INT, indices);
}

void GraphicsContext::drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo)
{
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
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
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
					glColorPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
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
					glTexCoordPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				break;
		}
	}

	// Draw vbo
	glDrawElements(type, ibo->getSize(), GL_UNSIGNED_INT, 0);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GraphicsContext::drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount)
{
	setupContext();

	// Get vertices and vertex data
	VertexFormat fmt = vertices->getFormat();
	char *vertexData = new char[vertexCount * fmt.getVertexSizeInBytes()];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * fmt.getVertexSizeInBytes());
	}
			
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
					glVertexPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
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
					glColorPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
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
					glTexCoordPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, vertexData + fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				break;
		}
	}

	// Draw primitives
	glDrawArrays(type, 0, vertexCount);
}

void GraphicsContext::drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo)
{
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
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
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
					glColorPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
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
					glTexCoordPointer(fmt.getElementCount(attrib), fmt.getDataType(attrib), stride, (void*)fmt.getAttributeOffset(attrib));
				}
				else
				{
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				break;
		}
	}

	// Draw vbo
	glDrawArrays(type, 0, vbo->getSize());

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GraphicsContext::drawRectangle(const float x, const float y, const float width, const float height, const Color &color)
{
	Vertex vertices[4];

	vertices[0].set4f(xd::VERTEX_POSITION, x,			y);
	vertices[1].set4f(xd::VERTEX_POSITION, x + width,	y);
	vertices[2].set4f(xd::VERTEX_POSITION, x,			y + height);
	vertices[3].set4f(xd::VERTEX_POSITION, x + width,	y + height);

	vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[1].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[2].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[3].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	
	vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);
	vertices[1].set4f(xd::VERTEX_TEX_COORD, 1.0f, 1.0f);
	vertices[2].set4f(xd::VERTEX_TEX_COORD, 0.0f, 0.0f);
	vertices[3].set4f(xd::VERTEX_TEX_COORD, 1.0f, 0.0f);

	drawPrimitives(PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
}

void GraphicsContext::drawRectangle(const Vector2 &pos, const Vector2 &size, const Color &color)
{
	drawRectangle(pos.x, pos.y, size.x, size.y, color);
}

void GraphicsContext::drawRectangle(const Rect &rect, const Color &color)
{
	drawRectangle(rect.position.x, rect.position.y, rect.size.x, rect.size.y, color);
}

void GraphicsContext::drawCircle(const float x, const float y, const float radius, const uint segments, const Color &color)
{
	Vertex *vertices = new Vertex[segments+2];

	vertices[0].set4f(xd::VERTEX_POSITION, x, y);
	vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);

	for(uint i = 1; i < segments+2; ++i)
	{
		float r = (2.0f*PI*i)/segments;
		vertices[i].set4f(VERTEX_POSITION, x + cos(r)*radius, y + sin(r)*radius);
		vertices[i].set4ub(VERTEX_COLOR, color.r, color.g, color.b, color.a);
		vertices[i].set4f(VERTEX_TEX_COORD, (1 + cos(r))/2.0f, 1.0f - (1 + sin(r))/2.0f);
	}

	drawPrimitives(PRIMITIVE_TRIANGLE_FAN, vertices, segments+2);

	delete[] vertices;
}

void GraphicsContext::drawCircle(const Vector2 &center, const float radius, const uint segments, const Color &color)
{
	drawCircle(center.x, center.y, radius, segments, color);
}

END_XD_NAMESPACE