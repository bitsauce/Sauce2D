//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		Macro#byte (C)

#include "android_render.h"
#include "android_engine.h"

#include <android/native_window.h>

#include "x2d/iosystem.h"
#include "x2d/math.h"
#include "x2d/scripts.h"
#include "x2d/engine.h"
#include "x2d/assetLoader.h"

bool is_big_endian()
{
    union {
        uint i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}

uint toGLBlend(const X2DBlendFunc val)
{
	switch(val) {
	case X2D_ZeroBlend:				return GL_ZERO;
	case X2D_OneBlend:				return GL_ONE;
	case X2D_SrcColorBlend:			return GL_SRC_COLOR;
	case X2D_OneMinusSrcColorBlend:	return GL_ONE_MINUS_SRC_COLOR;
	case X2D_SrcAlphaBlend:			return GL_SRC_ALPHA;
	case X2D_OneMinusSrcAlphaBlend:	return GL_ONE_MINUS_SRC_ALPHA;
	case X2D_DstAlphaBlend:			return GL_DST_ALPHA;
	case X2D_OneMinusDstAlphaBlend:	return GL_ONE_MINUS_DST_ALPHA;
	case X2D_DstColorBlend:			return GL_DST_COLOR;
	case X2D_OneMinusDstColorBlend:	return GL_ONE_MINUS_DST_COLOR;
	case X2D_SrcAlphaSaturate:		return GL_SRC_ALPHA_SATURATE;
	}
	return GL_ZERO;
}

uint toGLDrawMode(const X2DDrawMode val)
{
	switch(val) {
	case X2D_DrawPoints: return GL_POINTS;
	case X2D_DrawLines: return GL_LINES;
	case X2D_DrawLineLoop: return GL_LINE_LOOP;
	case X2D_DrawTriangles: return GL_TRIANGLES;
	case X2D_DrawTriangleStrip: return GL_TRIANGLE_STRIP;
	case X2D_DrawTriangleFan: return GL_TRIANGLE_FAN;
	}
	return GL_TRIANGLES;
}

uint toGLBufferMode(const X2DVertexBufferMode val)
{
	switch(val) {
	case X2D_StreamBuffer: 
	case X2D_DynamicBuffer: return GL_DYNAMIC_DRAW;
	case X2D_StaticBuffer: return GL_STATIC_DRAW;
	}
	return GL_DYNAMIC_DRAW;
}

uint toGLFilterMode(const X2DTextureFilter val)
{
	switch(val) {
	case X2D_NearestFilter: return GL_NEAREST;
	case X2D_BilinearFilter: return GL_LINEAR;
	}
	return GL_NEAREST;
}

AndroidRender::AndroidRender(AppManager *manager) :
	m_nextFontId(0),
	m_currentFont(0),
	m_currentVertex(),
	m_currentFbo(0),
	m_currentBuffer(0),
	m_manager(manager)
{
	assert(manager);
}

AndroidRender::~AndroidRender()
{
	for(map<int, Font>::iterator itr = Fonts.begin(); itr != Fonts.end(); ++itr)
	{
		Font *font = &itr->second;
		glDeleteTextures(1, &font->texture);
	}
	Fonts.clear();
	
	// Delete all loaded textures
	for(map<int, Texture>::iterator itr = Textures.begin(); itr != Textures.end(); ++itr)
		glDeleteTextures(1, (GLuint*)itr->first);

	// Release all vertex buffers
	for(uint i = 0; i < VertexBuffers.size(); i++)
	{
		glDeleteBuffers(1, &i);
	}
}

void AndroidRender::beginDraw()
{
	// Clear front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup batch stack
	m_batchStack.push_back(Batch());
}

void AndroidRender::endDraw()
{
	drawFrontBatch();
    eglSwapBuffers(m_manager->display, m_manager->surface);
}

void AndroidRender::setViewport(const int x, const int y, const int w, const int h)
{
	glViewport(x, y, w, h);
}

void AndroidRender::setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)
{
	// Set orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(l, r, b, t, n, f);
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

void AndroidRender::getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f)
{
	// Get orthographic projection
	l = m_currentOrtho[0];
	r = m_currentOrtho[1];
	b = m_currentOrtho[2];
	t = m_currentOrtho[3];
	n = m_currentOrtho[4];
	f = m_currentOrtho[5];
}

void AndroidRender::addText(const vec2 pos, const string &text)
{
	// Draw text using current font
	if(m_currentFont == 0) {
		iosystem::error("OpenGLRender::drawText() No font is set!");
		return;
	}

	// Add space between lines
	float h = m_currentFont->fontSize/0.63f;
	float yOffset = 0.0f;
	float xOffset = 0.0f;
 
	// Split string lines
	string line;
	setTexture(m_currentFont->texture);
	begin();
	for(uint i = 0; i < text.size(); i++)
	{
		// Check for new line
		if(text[i] == '\n')
		{
			yOffset += h;
			xOffset = 0.0f;
			continue;
		}

		// Get char
		Char c = m_currentFont->chars[text[i]];

		// Draw char
		float px = pos.x + xOffset + c.pos.x;
		float py = pos.y + yOffset + c.pos.y + m_currentFont->fontSize;
		addRect(vec2(px, py), c.size, c.texCoord0, c.texCoord1);
		/*setTexCoord(c.texCoord0.x, c.texCoord1.y); addVertex(px, py);
		setTexCoord(c.texCoord0.x, c.texCoord0.y); addVertex(px, py + c.size.y);
		setTexCoord(c.texCoord1.x, c.texCoord0.y); addVertex(px + c.size.x, py + c.size.y);
		setTexCoord(c.texCoord1.x, c.texCoord1.y); addVertex(px + c.size.x, py);*/

		// Apply advance
		xOffset += c.advance;
	}
	end();
	resetTexture();
}

void AndroidRender::addText(const float x, const float y, const string &text)
{
	addText(vec2(x, y), text);
}

void AndroidRender::setDepth(const int depth)
{
	m_currentState.depth = depth;
}

void AndroidRender::begin(const X2DDrawMode mode)
{
	// Check for buffer
	if(m_currentBuffer)
		return;

	// Set draw mode
	m_currentState.drawMode = toGLDrawMode(mode);

	// Find buffer
	if(m_batchStack.back().buffers.find(m_currentState) == m_batchStack.back().buffers.end())
	{
		// No buffer exists, create one
		m_batchStack.back().buffers[m_currentState] = m_currentBuffer = new DynamicBuffer;
	}else{
		// Use existing buffer
		m_currentBuffer = static_cast<DynamicBuffer*>(m_batchStack.back().buffers[m_currentState]);
	}
}

void AndroidRender::addVertex(const vec2 pos)
{
	// Make sure we have a buffer bound
	if(!m_currentBuffer)
		return;

	// Add vertex to batch
	m_currentVertex.position = pos;
	m_currentBuffer->addVertex(m_currentVertex);
}

void AndroidRender::addVertex(const float x, const float y)
{
	addVertex(vec2(x, y));
}

void AndroidRender::addRect(const vec2 pos, const vec2 size, const vec2 coord0, const vec2 coord1, const bool center)
{
	// Top-left
	if(center) {
		m_currentVertex.position = pos-size/2;
		m_currentVertex.texCoord.set(coord0.x, coord1.y);
		m_currentBuffer->addVertex(m_currentVertex);
	}else{
		m_currentVertex.position = pos;
		m_currentVertex.texCoord.set(coord0.x, coord1.y);
		m_currentBuffer->addVertex(m_currentVertex);
	}
		
	// Top-right
	m_currentVertex.position.x += size.x;
	m_currentVertex.texCoord.set(coord1.x, coord1.y);
	m_currentBuffer->addVertex(m_currentVertex);
		
	// Bottom-right
	m_currentVertex.position.y += size.y;
	m_currentVertex.texCoord.set(coord1.x, coord0.y);
	m_currentBuffer->addVertex(m_currentVertex);
		
	// Bottom-left
	m_currentVertex.position.x -= size.x;
	m_currentVertex.texCoord.set(coord0.x, coord0.y);
	m_currentBuffer->addVertex(m_currentVertex);
}

void AndroidRender::addRect(const float x, const float y, const float w, const float h, const float cx0, const float cy0, const float cx1, const float cy1, const bool center)
{
	addRect(vec2(x, y), vec2(w, h), vec2(cx0, cy0), vec2(cx1, cy1), center);
}

void AndroidRender::end()
{
	m_currentBuffer = 0;
}

void AndroidRender::beginClip(const float x, const float y, const float width, const float height)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor((int)x, (int)y, (int)width, (int)height);
}

void AndroidRender::endClip()
{
	glDisable(GL_SCISSOR_TEST);
}

void AndroidRender::setTexture(const int texId)
{
	if(texId < 0)
		return;

	// Update current draw state
	m_currentState.texture = texId;
}

void AndroidRender::setShader(const int shaderId)
{
	// Set current shader program
	m_currentState.shader.program = shaderId;
	if(shaderId == 0)
	{
		m_currentState.shader.textureCount = 0;
		m_currentState.shader.uniforms.clear();
	}
}

void AndroidRender::setTexCoord(const vec2 coord)
{
	m_currentVertex.texCoord = coord;
}

void AndroidRender::setColor(const vec4 color)
{
	m_currentVertex.color = color;
}

void AndroidRender::setBlendFunc(const X2DBlendFunc src, const X2DBlendFunc dst)
{
	m_currentState.srcBlendAlpha = m_currentState.srcBlendColor = toGLBlend(src);
	m_currentState.dstBlendAlpha = m_currentState.dstBlendColor = toGLBlend(dst);
}

void AndroidRender::setBlendFuncSep(const X2DBlendFunc srcc, const X2DBlendFunc dstc, const X2DBlendFunc srca, const X2DBlendFunc dsta)
{
	m_currentState.srcBlendColor = srcc;
	m_currentState.dstBlendColor = dstc;
	m_currentState.srcBlendAlpha = srca;
	m_currentState.dstBlendAlpha = dsta;
}

void AndroidRender::resetDepth()
{
	m_currentState.depth = 0;
}

void AndroidRender::resetColor()
{
	m_currentVertex.color = vec4(1.0f);
}

void AndroidRender::resetTexture()
{
	m_currentState.texture = 0;
}

void AndroidRender::resetBlendFunc()
{
	m_currentState.srcBlendColor = GL_SRC_ALPHA;
	m_currentState.dstBlendColor = GL_ONE_MINUS_SRC_ALPHA;
	m_currentState.srcBlendAlpha = GL_SRC_ALPHA;
	m_currentState.dstBlendAlpha = GL_ONE_MINUS_SRC_ALPHA;
}

void AndroidRender::resetShader()
{
	setShader(0);
}

void AndroidRender::reset()
{
	m_currentState.depth = 0;
	m_currentVertex.color = vec4(1.0f);
	m_currentState.texture = 0;
	m_currentState.srcBlendColor = GL_SRC_ALPHA;
	m_currentState.dstBlendColor = GL_ONE_MINUS_SRC_ALPHA;
	m_currentState.srcBlendAlpha = GL_SRC_ALPHA;
	m_currentState.dstBlendAlpha = GL_ONE_MINUS_SRC_ALPHA;
	setShader(0);
}

void AndroidRender::pushTransform(const vec2 t, const vec4 r, const vec2 s)
{
	//Matrix4 mat;
	//mat.translate(px, py, pz);
	//mat.rotate(ang, rx, ry, rz);
	//mat.scale(sx, sy, sz);
	//m_matrixStack.push_back(m_matrixStack.back()*mat);
	glPushMatrix();
	glTranslatef(t.x, t.y, 0.0f);
	glRotatef(r.w, r.x, r.y, r.z);
	glScalef(s.x, s.y, 1.0f);
}

void AndroidRender::popTransform()
{
	//if(m_matrixStack.size() == 1)
	//	error("Cannot pop the identity matrix");
	//m_matrixStack.pop_back();
	glPopMatrix();
}

const int floatSize = sizeof(float);

void AndroidRender::drawFrontBatch()
{
	// Enable client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Iterate batches and draw them
	Batch *frontBatch = &m_batchStack.back();
	for(map<State, DrawBuffer*>::iterator batchItr = frontBatch->buffers.begin(); batchItr != frontBatch->buffers.end(); ++batchItr)
	{
		// Bind the batch texture
		const State *state = &batchItr->first;
		glBindTexture(GL_TEXTURE_2D, state->texture);
		
#ifdef USE_GLES_2_0
		// Set blend mode
		glBlendFuncSeparate(state->srcBlendColor, state->dstBlendColor,
			state->srcBlendAlpha, state->dstBlendAlpha); 

		// Bind shader
		//if(state->shader.program > 0)
		{
			const ShaderState *shader = &state->shader;
			glUseProgram(shader->program);
			
			// Set all uniforms
			for(uint i = 0; i < shader->uniforms.size(); i++)
			{
				const Uniform *uniform = &shader->uniforms[i];
				switch(uniform->type)
				{
				case Uniform1i: glUniform1i(uniform->loc, ((int*)uniform->data)[0]); break;
				case Uniform2i: glUniform2i(uniform->loc, ((int*)uniform->data)[0], ((int*)uniform->data)[1]); break;
				case Uniform3i: glUniform3i(uniform->loc, ((int*)uniform->data)[0], ((int*)uniform->data)[1], ((int*)uniform->data)[2]); break;
				case Uniform4i: glUniform4i(uniform->loc, ((int*)uniform->data)[0], ((int*)uniform->data)[1], ((int*)uniform->data)[2], ((int*)uniform->data)[3]); break;
				case Uniform1f: glUniform1f(uniform->loc, ((float*)uniform->data)[0]); break;
				case Uniform2f: glUniform2f(uniform->loc, ((float*)uniform->data)[0], ((float*)uniform->data)[1]); break;
				case Uniform3f: glUniform3f(uniform->loc, ((float*)uniform->data)[0], ((float*)uniform->data)[1], ((float*)uniform->data)[2]); break;
				case Uniform4f: glUniform4f(uniform->loc, ((float*)uniform->data)[0], ((float*)uniform->data)[1], ((float*)uniform->data)[2], ((float*)uniform->data)[3]); break;
				case UniformTexture:
					{
						int target = ((int*)uniform->data)[1];
						glActiveTexture(GL_TEXTURE0+target);
						glBindTexture(GL_TEXTURE_2D, ((int*)uniform->data)[0]);
						glUniform1i(uniform->loc, target);
					}
					break;
				}
				delete[] uniform->data;
			}
		}
#endif

		// Get vertices and vertex data
		DrawBuffer *buffer = batchItr->second;
		if(buffer->type == DynamicBufferType)
		{
			DynamicBuffer *dynbuf = static_cast<DynamicBuffer*>(buffer);
			float *vertexData = (float*)dynbuf->data();

			// Draw arrays
			glVertexPointer(2, GL_FLOAT, 8*floatSize, vertexData);
			glColorPointer(4, GL_FLOAT, 8*floatSize, vertexData + 2);
			glTexCoordPointer(2, GL_FLOAT, 8*floatSize, vertexData + 6);

			glDrawArrays(state->drawMode, 0, dynbuf->vertexCount());
		}else{
			StaticBuffer *vbo = static_cast<StaticBuffer*>(buffer);

			glLoadMatrixf(vbo->transform);

			glBindBuffer(GL_ARRAY_BUFFER, state->vbo);

			glVertexPointer(2, GL_FLOAT, 8*floatSize, (void*)(0*floatSize));
			glColorPointer(4, GL_FLOAT, 8*floatSize, (void*)(2*floatSize));
			glTexCoordPointer(2, GL_FLOAT, 8*floatSize, (void*)(6*floatSize));

			glDrawArrays(vbo->drawMode, vbo->begin, vbo->count);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glLoadIdentity();
		}
		delete buffer;
	}

	// Reset current batch
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable client state
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Clear batches
	m_batchStack.pop_back();
}

int AndroidRender::createTexture(const int width, const int height, const X2DTextureFilter filter, const bool mipmap)
{
	// Generate texture
	uint texid;
	glGenTextures(1, &texid);

	// Setup texture
	Texture *texture = &Textures[texid];
	texture->width = width;
	texture->height = height;
	
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)vector<uint>(width*height, 0).data());
	if(mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter) == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilterMode(filter));
	glBindTexture(GL_TEXTURE_2D, 0);

	// Return tex id
	return texid;
}

void AndroidRender::deleteTexture(const int texId)
{
	// Make sure it is a valid id
	if(Textures.find(texId) == Textures.end())
		return;

	// Delete the texture
	glDeleteTextures(1, (GLuint*)texId);
	Textures.erase(texId);
}

void AndroidRender::setTextureFilter(const int texId, const X2DTextureFilter filter, const bool mipmap)
{
	glBindTexture(GL_TEXTURE_2D, texId);
	if(mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter) == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilterMode(filter));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AndroidRender::setTextureData(const int texId, const int width, const int height, Array &data, const bool mipmap)
{
	if(data.GetSize() != uint(width*height))
	{
		iosystem::error("Error: gfxSetTextureData(): Invalid data size (%i != %i)", data.GetSize(), width*height);
		return;
	}

	// Set texture data
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data.At(0));
#ifdef USE_GLES_2_0
	if(mipmap) glGenerateMipmap(GL_TEXTURE_2D);
#endif
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set dimentions
	Textures[texId].width = width;
	Textures[texId].height = height;
}

#include "x2d/app.h"
#include "android_asset_loader.h"

#ifdef USE_GLES_2_0
int AndroidRender::loadShader(const string &vertFilePath, const string &fragFilePath)
{
	// Create vertex and fragment shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shader files to string
	const char *vertShaderBuffer, *fragShaderBuffer;
	AndroidAssetLoader* assetLoader = (AndroidAssetLoader*)m_manager->engine->assetLoader;
	assetLoader->loadAsset(vertFilePath.c_str(), &vertShaderBuffer);
	assetLoader->loadAsset(fragFilePath.c_str(), &fragShaderBuffer);

	// Default vars
    int result = 0;
    int logLength;

    // Compile vertex shader
	iosystem::print("Compiling vertex shader: %s", vertFilePath.c_str());
	glShaderSource(vertShader, 1, &vertShaderBuffer, NULL);
    glCompileShader(vertShader);

    // Validate vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    char* compileLog = new char[logLength];
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
	{
		iosystem::print("\tCompile error: %s", compileLog);
		delete[] vertShaderBuffer;
		delete[] fragShaderBuffer;
		return -1;
	}

    // Compile fragment shader
    iosystem::print("Compiling fragment shader: %s", fragFilePath.c_str());
    glShaderSource(fragShader, 1, &fragShaderBuffer, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
	{
		iosystem::print("\tCompile error: %s", compileLog);
		delete[] vertShaderBuffer;
		delete[] fragShaderBuffer;
		return -1;
	}

    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
    char* programLog = new char[(logLength > 1) ? logLength : 1];
    glGetProgramInfoLog(program, logLength, NULL, programLog);

	// Print program error to console
	if(logLength > 1)
	{
		iosystem::print("\tCompile error: %s", compileLog);
		delete[] vertShaderBuffer;
		delete[] fragShaderBuffer;
		return -1;
	}

	// Delete shader buffers as they are loaded into the shader program
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

	// Delete log buffers
	delete[] compileLog;
	delete[] vertShaderBuffer;
	delete[] fragShaderBuffer;
	delete[] programLog;

	// Return shader program
    return program;
}

void AndroidRender::setUniform1i(const string &name, const int v0)
{
	// Set uniform
	Uniform uniform(Uniform1i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[1];
	((int*)uniform.data)[0] = v0;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform2i(const string &name, const int v0, const int v1)
{
	// Set uniform
	Uniform uniform(Uniform2i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[2];
	((int*)uniform.data)[0] = v0;
	((int*)uniform.data)[1] = v1;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform3i(const string &name, const int v0, const int v1, const int v2)
{
	// Set uniform
	Uniform uniform(Uniform3i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[3];
	((int*)uniform.data)[0] = v0;
	((int*)uniform.data)[1] = v1;
	((int*)uniform.data)[2] = v2;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3)
{
	// Set uniform
	Uniform uniform(Uniform4i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[4];
	((int*)uniform.data)[0] = v0;
	((int*)uniform.data)[1] = v1;
	((int*)uniform.data)[2] = v2;
	((int*)uniform.data)[3] = v3;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform1f(const string &name, const float v0)
{
	// Set uniform
	Uniform uniform(Uniform1f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[1];
	((float*)uniform.data)[0] = v0;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform2f(const string &name, const float v0, const float v1)
{
	// Set uniform
	Uniform uniform(Uniform2f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[2];
	((float*)uniform.data)[0] = v0;
	((float*)uniform.data)[1] = v1;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform3f(const string &name, const float v0, const float v1, const float v2)
{
	// Set uniform
	Uniform uniform(Uniform3f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[3];
	((float*)uniform.data)[0] = v0;
	((float*)uniform.data)[1] = v1;
	((float*)uniform.data)[2] = v2;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3)
{
	// Set uniform
	Uniform uniform(Uniform4f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[4];
	((float*)uniform.data)[0] = v0;
	((float*)uniform.data)[1] = v1;
	((float*)uniform.data)[2] = v2;
	((float*)uniform.data)[3] = v3;
	m_currentState.shader.uniforms.push_back(uniform);
}

void AndroidRender::setUniformTexture(const string &name, const int texId)
{
	// Set uniform
	int target = m_currentState.shader.textureCount++;
	Uniform uniform(UniformTexture);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[2];
	((int*)uniform.data)[0] = texId;
	((int*)uniform.data)[1] = target;
	m_currentState.shader.uniforms.push_back(uniform);
}
#endif

int AndroidRender::loadImage(const string &filePath, const X2DTextureFilter filter, const bool mipmap)
{
	// Load texture data
	int width, height;
	Array* arr = loadImageData(filePath, width, height);
	if(arr->GetSize() == 0)
		return -1;

	// Generate texture
	int texture = createTexture(width, height, filter, mipmap);

	// Set texture data
	setTextureData(texture, width, height, *arr, mipmap);
	
	// Return texture id
	return texture;
}

Array *AndroidRender::loadImageData(const string &filePath, int &w, int &h)
{
	// Create script array
    Array *arr = CreateArray("uint", 0);
	
	// Load image from assets
	X2DAssetLoader *assetLoader = m_manager->engine->assetLoader;
	uint *data = 0;
	uint width = 0, height = 0;
	int e = assetLoader->loadAssetAsImage(filePath.c_str(), &data, width, height);
	if(e != X2D_OK)
	{
		iosystem::error("Unable to load image with error code: %i", e);
		return arr;
	}

	// Fill the array with the image data
	arr->Resize(width*height);
	for(uint y = 0; y < height; y++)
	{
		for(uint x = 0; x < width; x++)
		{
			uint i = x+y*width;
			arr->SetValue(i, &data[i]);
		}
	}

	// Set out width and out height
	w = width;
	h = height;

	// Clean up
	delete[] data;

	// Return array
	return arr;
}

void AndroidRender::clearTexture(const int texId)
{
	// Blank pixel data
	Texture *texture = &Textures[texId];

	// Clear texture
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height, GL_RGBA, GL_UNSIGNED_BYTE, vector<uint>(texture->width*texture->height, 0).data());
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glClearTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, &clearColor); // The better way, although requires OpenGL 4.3
}

void AndroidRender::getTextureSize(const int texId, int &w, int &h)
{
	// Check for bound texture
	if(Textures.find(texId) == Textures.end())
		return;

	// Get texture
	w = Textures[texId].width;
	h = Textures[texId].height;
}

int AndroidRender::copyToTexture(const int texId, const int x, const int y, const int width, const int height)
{
	return 0;
	// Generate texture
	GLuint id;
	glGenTextures(1, &id);

	// Generate and copy texture
	glBindTexture(GL_TEXTURE_2D, id);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, width, height, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Setup texture
	Texture *texture = &Textures[id];
	texture->width = width;
	texture->height = height;

	// Create texture object
	return id;
}

void AndroidRender::clearColorBit()
{
	// Clear the color
	glClear(GL_COLOR_BUFFER_BIT);
}

#ifdef USE_GLES_2_0
int AndroidRender::createFrameBuffer()
{
	GLuint id;
	glGenFramebuffers(1, &id);
	return id;
}

void AndroidRender::bindFrameBuffer(const int fboId)
{
	// Check for batch draw
	if(m_currentFbo != fboId)
	{
		if(fboId != 0)
		{
			// Add new batch
			m_batchStack.push_back(Batch());
		}else{
			// Draw current batch
			drawFrontBatch();
		}
	}

	// Bind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	m_currentFbo = fboId;
}

void AndroidRender::attachTextureToFrameBuffer(const int textureId)
{
	// Attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
}
#endif

int AndroidRender::createVertexBuffer()
{
	GLuint id;
	glGenBuffers(1, &id);
	VertexBuffers.push_back(id);
	return id;
}

void AndroidRender::bindVertexBuffer(const int vboId)
{
	// Bind frame buffer
	m_currentState.vbo = vboId;
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

void AndroidRender::setVertexBufferData(const Array &data, const X2DVertexBufferMode mode)
{
	glBufferData(GL_ARRAY_BUFFER, data.GetSize()*sizeof(float), data.At(0), toGLBufferMode(mode));
}

void AndroidRender::setVertexBufferSubData(const int offset, const Array &data)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(float), data.GetSize()*sizeof(float), data.At(0));
}

void AndroidRender::drawVertexBuffer(const X2DDrawMode drawMode, const int begin, const int count)
{
	// Make sure no buffer is already bound
	if(m_currentBuffer)
		return;

	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	// Allocate static buffer
	StaticBuffer *buffer = new StaticBuffer;
	buffer->drawMode = drawMode;
	buffer->begin = begin;
	buffer->count = count;
	memcpy(buffer->transform, m, 16*floatSize);
	m_batchStack.back().buffers[m_currentState] = buffer;
}

int AndroidRender::getDrawCallCount()
{
	if(m_batchStack.size() == 0) return 0;
	return m_batchStack.back().buffers.size();
}

int AndroidRender::initGL()
{
    // Initialize OpenGL ES and EGL
	iosystem::print("** Initialize OpenGL ES 1.1 **");

    // Set GLES config attributes
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
			//EGL_ALPHA_SIZE, 8,
            EGL_NONE
    };

	// Variable definitions
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(display != EGL_NO_DISPLAY);

	// Initialize GLES
	if(!eglInitialize(display, 0, 0))
	{
		// Display initialization failed
        iosystem::error("Unable to initialize GLES display");
		return -1;
	}

    // Find the closest configuration // TODO: Selection process may be improved
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(m_manager->android->window, 0, 0, format);

	// Create window surface
    surface = eglCreateWindowSurface(display, config, m_manager->android->window, NULL);
    
	// Initialize context and make current
	context = eglCreateContext(display, config, NULL, NULL);
    if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		// Context initialization failed
        iosystem::error("Unable to create GLES context");
		return -1;
    }

	// Query a surface for our render
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	// Init OpenGL
	glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable alpha
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	// Enable blend
	glEnable(GL_BLEND);
	
	// Disable depth test
    glDisable(GL_DEPTH_TEST);

	// Set OpenGL hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Set app manager variables
    m_manager->display = display;
    m_manager->context = context;
    m_manager->surface = surface;
    m_manager->width = w;
    m_manager->height = h;

	// Success!
	iosystem::success("OpenGL ES Initialized Successfully");

    return 0;
}

void AndroidRender::destroyGL()
{
	iosystem::print("** Destroying OpenGL ES **");

	if(m_manager->display != EGL_NO_DISPLAY)
	{
        eglMakeCurrent(m_manager->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if(m_manager->context != EGL_NO_CONTEXT)
            eglDestroyContext(m_manager->display, m_manager->context);
        if(m_manager->surface != EGL_NO_SURFACE)
            eglDestroySurface(m_manager->display, m_manager->surface);
        eglTerminate(m_manager->display);
    }

    m_manager->display = EGL_NO_DISPLAY;
    m_manager->context = EGL_NO_CONTEXT;
    m_manager->surface = EGL_NO_SURFACE;
}