#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

// Default shader. Used when no shader is set.
Resource<Shader> GraphicsContext::s_defaultShader = 0;

// Default texture. Empty texture used when no texture is set.
Resource<Texture2D> GraphicsContext::s_defaultTexture = 0;

// Vertex array object
GLuint GraphicsContext::s_vao = 0;
GLuint GraphicsContext::s_vbo = 0;
GLuint GraphicsContext::s_ibo = 0;

GraphicsContext::GraphicsContext(Window *window) :
	m_window(window),
	m_width(0),
	m_height(0),
	m_renderTarget(nullptr),
	m_shader(nullptr),
	m_texture(nullptr),
	m_blendState(BlendState::PRESET_ALPHA_BLEND)
{
	m_context = SDL_GL_CreateContext(window->getSDLHandle());
}

GraphicsContext::~GraphicsContext()
{
	glDeleteBuffers(1, &s_vbo);
	glDeleteVertexArrays(1, &s_vao);
	SDL_GL_DeleteContext(m_context);
}

void GraphicsContext::enable(const Capability cap)
{
	glEnable(cap);
}

void GraphicsContext::disable(const Capability cap)
{
	glDisable(cap);
}


void GraphicsContext::setPointSize(const float pointSize)
{
	glPointSize(pointSize);
}

void GraphicsContext::setLineWidth(const float lineWidth)
{
	glLineWidth(lineWidth);
}

void GraphicsContext::clear(const uint mask, const Color &fillColor)
{
	if(mask & COLOR_BUFFER) glClearColor(fillColor.getR() / 255.0f, fillColor.getG() / 255.0f, fillColor.getB() / 255.0f, fillColor.getA() / 255.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(fillColor.getR() / 255.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(fillColor.getR() / 255.0f);
	glClear(mask);
	if(mask & COLOR_BUFFER) glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if(mask & DEPTH_BUFFER) glClearDepth(0.0f);
	if(mask & STENCIL_BUFFER) glClearStencil(0.0f);
}

void GraphicsContext::enableScissor(const int x, const int y, const int w, const int h)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
}

void GraphicsContext::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void GraphicsContext::setRenderTarget(RenderTarget2D *renderTarget)
{
	// Setup viewport and projection
	if(m_renderTarget != renderTarget)
	{
		if(renderTarget)
		{
			// Bind new render target
			m_renderTarget = renderTarget;
			m_renderTarget->bind();

			// Resize viewport
			resizeViewport(m_renderTarget->m_width, m_renderTarget->m_height, true);
		}
		else if(m_renderTarget)
		{
			// Unbind render target
			m_renderTarget->unbind();
			m_renderTarget = 0;

			// Resize viewport
			resizeViewport(m_window->getWidth(), m_window->getHeight());
		}
	}
}

void GraphicsContext::setTransformationMatrix(const Matrix4 &projmat)
{
	while(!m_transformationMatrixStack.empty()) m_transformationMatrixStack.pop();
	m_transformationMatrixStack.push(projmat);
}

Matrix4 GraphicsContext::getTransformationMatrix() const
{
	if(m_transformationMatrixStack.empty()) return Matrix4();
	return m_transformationMatrixStack.top();
}

void GraphicsContext::pushMatrix(const Matrix4 &mat)
{
	if(m_transformationMatrixStack.empty()) m_transformationMatrixStack.push(mat);
	else m_transformationMatrixStack.push(m_transformationMatrixStack.top() * mat);
}

void GraphicsContext::popMatrix()
{
	if(m_transformationMatrixStack.empty()) return;
	m_transformationMatrixStack.pop();
}

void GraphicsContext::setTexture(Resource<Texture2D> texture)
{
	m_texture = texture;
}

Resource<Texture2D> GraphicsContext::getTexture() const
{
	return m_texture;
}

void GraphicsContext::setShader(Resource<Shader> shader)
{
	m_shader = shader;
}

Resource<Shader> GraphicsContext::getShader() const
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

void GraphicsContext::saveScreenshot(string path)
{
	// Get frame buffer data
	uchar *data = new uchar[m_width * m_height * 4];
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glReadBuffer(GL_BACK);

	// NOTE: This function is not tested!
	Pixmap pixmap(m_width, m_height, data);
	pixmap.flipY();
	pixmap.exportToFile(path);

	delete[] data;
}

// Orthographic projection
void GraphicsContext::resizeViewport(const uint w, const uint h, const bool flipY)
{
	// Set size
	m_width = w;
	m_height = h;

	// Set orthographic projection
	float l = 0.0f,
		r = (float) m_width,
		b = flipY ? 0.0f : (float) m_height,
		t = flipY ? (float)m_height : 0.0f,
		n = -1.0f,
		f = 1.0f;

	float projMat[16] = {
		2.0f / (r - l),		0.0f,					0.0f,				-((r + l) / (r - l)),
		0.0f,				2.0f / (t - b),			0.0f,				-((t + b) / (t - b)),
		0.0f,				0.0f,					-2.0f / (f - n),	-((f + n) / (f - n)),
		0.0f,				0.0f,					0.0f,				1.0f
	};

	m_projectionMatrix.set(projMat);

	// Set model-view to identity
	setTransformationMatrix(Matrix4());

	// Set viewport
	glViewport(0, 0, m_width, m_height);
}

void GraphicsContext::setProjectionMatrix(const Matrix4 matrix)
{
	m_projectionMatrix = matrix;
}

void GraphicsContext::setupContext()
{
	// Set blend func
	glBlendFuncSeparate(m_blendState.m_src, m_blendState.m_dst, m_blendState.m_alphaSrc, m_blendState.m_alphaDst);

	Resource<Shader> shader = m_shader;
	if(!shader)
	{
		shader = s_defaultShader;
		shader->setSampler2D("u_Texture", m_texture == 0 ? s_defaultTexture : m_texture);
	}

	// Enable shader
	glUseProgram(shader->m_id);

	// Set projection matrix
	Matrix4 modelViewProjection = m_projectionMatrix * m_transformationMatrixStack.top();
	shader->setUniformMatrix4f("u_ModelViewProj", modelViewProjection.get());

	GLuint target = 0;

	// Set all uniforms
	for(map<string, Shader::Uniform*>::iterator itr = shader->m_uniforms.begin(); itr != shader->m_uniforms.end(); ++itr)
	{
		const Shader::Uniform *uniform = itr->second;
		switch(uniform->type)
		{
			case GL_INT: case GL_BOOL: glUniform1i(uniform->loc, ((GLint*) uniform->data)[0]); break;
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
	}
}

void GraphicsContext::drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount)
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
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSizeInBytes, vertexData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, GL_DYNAMIC_DRAW);

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
	glDrawElements(type, indexCount, GL_UNSIGNED_INT, 0);

	// Reset vbo buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GL_CHECK_ERROR

		// Release vertex data
		delete[] vertexData;
}

void GraphicsContext::drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo)
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

	GL_CHECK_ERROR
}

void GraphicsContext::drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount)
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

	GL_CHECK_ERROR

		// Release vertex data
		delete[] vertexData;
}

void GraphicsContext::drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo)
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

	GL_CHECK_ERROR
}

void GraphicsContext::drawRectangle(const float x, const float y, const float width, const float height, const Color &color, const TextureRegion &textureRegion)
{
	// Make sure we have enought vertices
	if(m_vertices.size() < 4)
	{
		m_vertices.resize(4);
	}

	m_vertices[0].set2f(VERTEX_POSITION, x, y);
	m_vertices[1].set2f(VERTEX_POSITION, x, y + height);
	m_vertices[2].set2f(VERTEX_POSITION, x + width, y);
	m_vertices[3].set2f(VERTEX_POSITION, x + width, y + height);

	m_vertices[0].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	m_vertices[1].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	m_vertices[2].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	m_vertices[3].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());

	m_vertices[0].set2f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv0.y);
	m_vertices[1].set2f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv1.y);
	m_vertices[2].set2f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv0.y);
	m_vertices[3].set2f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv1.y);

	drawPrimitives(PRIMITIVE_TRIANGLE_STRIP, &m_vertices[0], 4);
}

void GraphicsContext::drawRectangle(const Vector2F &pos, const Vector2F &size, const Color &color, const TextureRegion &textureRegion)
{
	drawRectangle(pos.x, pos.y, size.x, size.y, color, textureRegion);
}

void GraphicsContext::drawRectangle(const Rect<float> &rect, const Color &color, const TextureRegion &textureRegion)
{
	drawRectangle(rect.position.x, rect.position.y, rect.size.x, rect.size.y, color, textureRegion);
}

void GraphicsContext::drawCircle(const float x, const float y, const float radius, const uint segments, const Color &color)
{
	// Make sure we have enought vertices
	if(m_vertices.size() < segments + 2)
	{
		m_vertices.resize(segments + 2);
	}

	m_vertices[0].set2f(VERTEX_POSITION, x, y);
	m_vertices[0].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	m_vertices[0].set2f(VERTEX_TEX_COORD, 0.5f, 0.5f);

	for(uint i = 1; i < segments + 2; ++i)
	{
		float r = (2.0f * PI * i) / segments;
		m_vertices[i].set2f(VERTEX_POSITION, x + cos(r) * radius, y + sin(r) * radius);
		m_vertices[i].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
		m_vertices[i].set2f(VERTEX_TEX_COORD, (1.0f + cos(r)) / 2.0f, (1.0f + sin(r)) / 2.0f);
	}

	drawPrimitives(PRIMITIVE_TRIANGLE_FAN, &m_vertices[0], segments + 2);
}

void GraphicsContext::drawCircle(const Vector2F &center, const float radius, const uint segments, const Color &color)
{
	drawCircle(center.x, center.y, radius, segments, color);
}

END_SAUCE_NAMESPACE
