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
	if(mask & STENCIL_BUFFER) glClearStencil(fillColor.r/255.0f);
	glClear(mask);
	if(mask & COLOR_BUFFER) glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(0.0f);
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

void GraphicsContext::setModelViewMatrix(const Matrix4 &projmat)
{
	while(!m_modelViewMatrixStack.empty()) m_modelViewMatrixStack.pop();
	m_modelViewMatrixStack.push(projmat);
}

Matrix4 GraphicsContext::getModelViewMatrix() const
{
	if(m_modelViewMatrixStack.empty()) return Matrix4();
	return m_modelViewMatrixStack.top();
}

void GraphicsContext::pushMatrix(const Matrix4 &mat)
{
	if(m_modelViewMatrixStack.empty()) m_modelViewMatrixStack.push(mat);
	else m_modelViewMatrixStack.push(m_modelViewMatrixStack.top() * mat);
}

void GraphicsContext::popMatrix()
{
	if(m_modelViewMatrixStack.empty()) return;
	m_modelViewMatrixStack.pop();
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
	float l = 0.0f,
		r = (float)m_width,
		b = (float)m_height,
		t = 0,
		n = -1.0f,
		f = 1.0f;

	float projMat[16] = {
		2.0f / ( r - l ),	0.0f,					0.0f,				- ( ( r + l ) / ( r - l ) ),
		0.0f,				2.0f / ( t - b ),		0.0f,				- ( ( t + b ) / ( t - b ) ),
		0.0f,				0.0f,					-2.0f / ( f - n ),	- ( ( f + n ) / ( f - n ) ),
		0.0f,				0.0f,					0.0f,				1.0f
	};

	m_projectionMatrix.set(projMat);

	// Set model-view to identity
	setModelViewMatrix(Matrix4());
	
	// Set viewport
	glViewport(0, 0, m_width, m_height);
}

void GraphicsContext::setupContext()
{
	// Set blend func
	glBlendFuncSeparate(m_blendState.m_src, m_blendState.m_dst, m_blendState.m_alphaSrc, m_blendState.m_alphaDst);

	ShaderPtr shader = m_shader;
	if (!shader)
	{
		shader = Graphics::s_defaultShader;
		shader->setSampler2D("u_Texture", m_texture == 0 ? Graphics::s_defaultTexture : m_texture);
	}

	// Enable shader
	glUseProgram(shader->m_id);

	// Set projection matrix
	Matrix4 modelViewProjection = m_projectionMatrix * m_modelViewMatrixStack.top();
	shader->setUniformMatrix4f("u_ModelViewProj", modelViewProjection.get());

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
		case GL_FLOAT_MAT4: glUniformMatrix4fv(uniform->loc, 1, GL_FALSE, (GLfloat*)uniform->data); break;
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_INT_SAMPLER_2D:
		case GL_SAMPLER_2D:
			{
				glActiveTexture(GL_TEXTURE0 + target);
				glBindTexture(GL_TEXTURE_2D, ((GLuint*)uniform->data)[0]);
				glUniform1i(uniform->loc, target++);
			}
			break;
		}
	}
}

void GraphicsContext::drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount)
{
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
	glBindBuffer(GL_ARRAY_BUFFER, Graphics::s_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSizeInBytes, vertexData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Graphics::s_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_DYNAMIC_DRAW);
			
	// Set array pointers
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch(attrib)
		{
		case VERTEX_POSITION:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(0);
			}
			break;

		case VERTEX_COLOR:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(1);
			}
			break;

		case VERTEX_TEX_COORD:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(2);
			}
			break;
		}
	}

	// Draw primitives
	glDrawElements(type, indexCount, GL_UNSIGNED_INT, 0);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(0);
			}
			break;

		case VERTEX_COLOR:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, stride, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(1);
			}
			break;

		case VERTEX_TEX_COORD:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*)fmt.getAttributeOffset(attrib));
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
}

void GraphicsContext::drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount)
{
	setupContext();

	// Get vertices and vertex data
	VertexFormat fmt = vertices->getFormat();
	int vertexSizeInBytes = fmt.getVertexSizeInBytes();
	char *vertexData = new char[vertexCount * vertexSizeInBytes];
	for (uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * vertexSizeInBytes);
	}

	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, Graphics::s_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSizeInBytes, vertexData, GL_DYNAMIC_DRAW);

	// Set array pointers
	for (int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		switch (attrib)
		{
		case VERTEX_POSITION:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(0);
			}
			break;

		case VERTEX_COLOR:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(1);
			}
			break;

		case VERTEX_TEX_COORD:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, vertexSizeInBytes, (void*)fmt.getAttributeOffset(attrib));
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
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(0);
			}
			break;

		case VERTEX_COLOR:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_TRUE, stride, (void*)fmt.getAttributeOffset(attrib));
			}
			else
			{
				glDisableVertexAttribArray(1);
			}
			break;

		case VERTEX_TEX_COORD:
			if (fmt.isAttributeEnabled(attrib))
			{
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, fmt.getElementCount(attrib), fmt.getDataType(attrib), GL_FALSE, stride, (void*)fmt.getAttributeOffset(attrib));
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
}

void GraphicsContext::drawRectangle(const float x, const float y, const float width, const float height, const Color &color)
{
	Vertex vertices[4];

	vertices[0].set4f(xd::VERTEX_POSITION, x,			y);
	vertices[1].set4f(xd::VERTEX_POSITION, x,			y + height);
	vertices[2].set4f(xd::VERTEX_POSITION, x + width,	y);
	vertices[3].set4f(xd::VERTEX_POSITION, x + width,	y + height);

	vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[1].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[2].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[3].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	
	vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);
	vertices[1].set4f(xd::VERTEX_TEX_COORD, 0.0f, 0.0f);
	vertices[2].set4f(xd::VERTEX_TEX_COORD, 1.0f, 1.0f);
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
	vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.5f, 0.5f);

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