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
#include "../../../window.h"

#include "../source/gfx/vertexbufferobject.h"
#include "../source/gfx/batch.h"

class GLvertexbuffer : public VertexBufferObject
{
	friend class OpenGL;
public:
	GLvertexbuffer() {
		glGenBuffers(1, &m_vboId);
		glGenBuffers(1, &m_iboId);
	}

	~GLvertexbuffer() {
		glDeleteBuffers(1, &m_vboId);
		glDeleteBuffers(1, &m_iboId);
	}

	void upload(const VertexBuffer *buffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(GL_ARRAY_BUFFER, buffer->vertices.size()*sizeof(Vertex), buffer->vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer->indices.size()*sizeof(uint), buffer->indices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void uploadSub(int offset, Vertex *vertices, int count)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(Vertex), count*sizeof(Vertex), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

private:
	GLuint m_vboId;
	GLuint m_iboId;
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

	// Set OpenGL hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void OpenGL::createContext(HWND window)
{
	// Describes the pixel format of the drawing surface
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;					// Version Number
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |	// Draws to a window
		PFD_SUPPORT_OPENGL |	// The format must support OpenGL
		PFD_DOUBLEBUFFER;		// Support for double buffering
	pfd.iPixelType = PFD_TYPE_RGBA;		// Uses an RGBA pixel format
	pfd.cColorBits = 24;				// 24 bits colors
	pfd.cAlphaBits = 8;					// 8 bits alpha

	// Get device context
	m_deviceContext = GetDC(window);
	if(!m_deviceContext)	
		assert("Unable to create rendering context");

	// Do Windows find a matching pixel format?
	int pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
	if(!pixelFormat)				
		assert("Unable to create rendering context");

	// Set the new pixel format
	if(!SetPixelFormat(m_deviceContext, pixelFormat, &pfd))			
		assert("Unable to create rendering context");

	// Create the OpenGL rendering context
	m_openGLContext = wglCreateContext(m_deviceContext);
	if(!m_openGLContext)
		assert("Unable to create rendering context");

	// Activate the rendering context
	if(!wglMakeCurrent(m_deviceContext, m_openGLContext))
		assert("Unable to create rendering context");
}

void OpenGL::destroyContext()
{
	if(m_openGLContext)
	{
		// Make the rendering context not current
		wglMakeCurrent(NULL, NULL);

		// Delete the OpenGL rendering context
		wglDeleteContext(m_openGLContext);
		m_openGLContext = NULL;
	}
}

void OpenGL::swapBuffers()
{
	SwapBuffers(m_deviceContext);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::setViewport(const Recti &rect)
{
	glViewport(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
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

const int FLOAT_SIZE = sizeof(float);
const int VERTEX_SIZE = sizeof(Vertex);

void OpenGL::renderBatch(const Batch &batch)
{
	// Enable client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	TextureVertexMap buffers = batch.m_buffers;
	Matrix4 mat = batch.m_projMatrix;
	glLoadMatrixf(mat.getTranspose());
	if(!batch.isStatic())
	{
		for(TextureVertexMap::iterator itr = buffers.begin(); itr != buffers.end(); ++itr)
		{
			const Texture *texture = itr->second->texture;
			glActiveTexture(GL_TEXTURE0);
			if(texture) {
				glBindTexture(GL_TEXTURE_2D, ((GLtexture*)texture)->m_id);
			}else{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			// Get vertices and vertex data
			float *vertexData = (float*)itr->second->vertices.data();
			uint *indexData = (uint*)itr->second->indices.data();

			// Draw arrays
			glVertexPointer(2, GL_FLOAT, VERTEX_SIZE, vertexData);
			glColorPointer(4, GL_FLOAT, VERTEX_SIZE, vertexData + 2);
			glTexCoordPointer(2, GL_FLOAT, VERTEX_SIZE, vertexData + 6);

			glDrawElements(GL_TRIANGLES, itr->second->indices.size(), GL_UNSIGNED_INT, indexData);
		}
	}else{
		for(TextureVertexMap::iterator itr = buffers.begin(); itr != buffers.end(); ++itr)
		{
			const Texture *texture = itr->second->texture;
			glActiveTexture(GL_TEXTURE0);
			if(texture) {
				glBindTexture(GL_TEXTURE_2D, ((GLtexture*)texture)->m_id);
			}else{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glBindBuffer(GL_ARRAY_BUFFER_ARB, ((GLvertexbuffer*)itr->second->vbo)->m_vboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLvertexbuffer*)itr->second->vbo)->m_iboId);

			glVertexPointer(2, GL_FLOAT, VERTEX_SIZE, (void*)(0*FLOAT_SIZE));
			glColorPointer(4, GL_FLOAT, VERTEX_SIZE, (void*)(2*FLOAT_SIZE));
			glTexCoordPointer(2, GL_FLOAT, VERTEX_SIZE, (void*)(6*FLOAT_SIZE));

			glDrawElements(GL_TRIANGLES, itr->second->indices.size(), GL_UNSIGNED_INT, 0);

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

/*void OpenGL::renderBatch(const Batch &batch)
{
	// Enable client state
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Bind the batch texture
	const State *state = &batchItr->first;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, state->texture);

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

	// Apply clipping
	if(state->clipRect.getArea() != 0)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(state->clipRect.getX(), state->clipRect.getY(), state->clipRect.getWidth(), state->clipRect.getHeight());
	}

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

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, state->vbo);

		glVertexPointer(2, GL_FLOAT, 8*floatSize, (void*)(0*floatSize));
		glColorPointer(4, GL_FLOAT, 8*floatSize, (void*)(2*floatSize));
		glTexCoordPointer(2, GL_FLOAT, 8*floatSize, (void*)(6*floatSize));

		glDrawArrays(vbo->drawMode, vbo->begin, vbo->count);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		glLoadIdentity();
	}
	delete buffer;
	glDisable(GL_SCISSOR_TEST);

	// Reset current batch
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable client state
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}*/

Texture *OpenGL::createTexture(const Pixmap &pixmap)
{
	return new GLtexture(pixmap);
}

Shader *OpenGL::createShader(const string &vertFilePath, const string &fragFilePath)
{
	return new GLshader(vertFilePath, fragFilePath);
}

VertexBufferObject *OpenGL::createVertexBufferObject()
{
	return new GLvertexbuffer();
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

#ifdef OLD
//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		Macro#byte (C)

#include "opengl.h"
#include "winassetloader.h"
#include "winapp.h"

#include "x2d/math.h"
#include "x2d/scripts.h"
#include "x2d/engine.h"
#include "x2d/iosystem.h"

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
	case X2D_StreamBuffer: return GL_STREAM_DRAW_ARB;
	case X2D_StaticBuffer: return GL_STATIC_DRAW_ARB;
	case X2D_DynamicBuffer: return GL_DYNAMIC_DRAW_ARB;
	}
	return GL_DYNAMIC_DRAW_ARB;
}

uint toGLFilterMode(const X2DTextureFilter val)
{
	switch(val) {
	case X2D_NearestFilter: return GL_NEAREST;
	case X2D_BilinearFilter: return GL_LINEAR;
	}
	return GL_NEAREST;
}

// TODO: Add safe-guards everywhere
// TODO: Check wheter or not there is a better solution for the text height hax
// TODO: Concider wheter or not it is posible to optimize the shader state

OpenGLRender::OpenGLRender(X2DEngine *engine, AssetLoader *assetLoader) :
	m_openGLContext(0),
	m_deviceContext(0),
	m_nextFontId(0),
	m_currentFont(0),
	m_currentVertex(),
	m_currentAtlas(0),
	m_currentFbo(0),
	m_currentBuffer(0),
	m_engine(engine),
	m_assetLoader(assetLoader)
{
	assert(engine);
}

OpenGLRender::~OpenGLRender()
{
	// TODO: Move to X2DRender destructor
	for(map<int, Font>::iterator itr = Fonts.begin(); itr != Fonts.end(); ++itr)
	{
		Font *font = &itr->second;
		glDeleteTextures(1, &font->texture);
	}
	Fonts.clear();

	// Delete all loaded textures
	for(map<int, Texture>::iterator itr = Textures.begin(); itr != Textures.end(); ++itr)
	{
		uint id = itr->first;
		glDeleteTextures(1, &id);
	}

	// Release all vertex buffers
	for(uint i = 0; i < VertexBuffers.size(); i++)
	{
		glDeleteBuffersARB(1, &i);
	}

	destroyContext();
}

void OpenGLRender::beginDraw()
{
	// Clear front buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup batch stack
	m_batchStack.push_back(Batch());
}

void OpenGLRender::endDraw()
{
	drawFrontBatch();
	SwapBuffers(m_deviceContext);
#ifdef _DEBUG
	int r = 0;
	if((r = glGetError()) != GL_NO_ERROR)
	{
		LOG("GL Error '%i' occurred!", r);
	}
#endif
}

void OpenGLRender::addText(const Vector2 pos, const string &text)
{
	// Draw text using current font
	if(m_currentFont == 0)
	{
		ERR("OpenGLRender::drawText() No font is set!");
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
		addRect(Vector2(px, py), c.size, c.texCoord0, c.texCoord1);
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

// Macro#byte (C)
// Inspired by: http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/
// Credits to the FreeType library
// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftsnames.h>

bool getFontFile(LPCTSTR fontName, string &fileName)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &hkey) == ERROR_SUCCESS)
	{
		// Get maximum buffer sizes
		ulong maxValue;
		ulong maxData;
		RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &maxValue, &maxData, NULL, NULL);
		maxValue++;
		maxData++;

		// Enumerate registry
		int i = 0;
		char *value = new char[maxValue];
		ulong valsize = maxValue;
		uchar *data = new uchar[maxData];
		ulong datasize = maxData;
		ulong datatype = REG_SZ;
		while(RegEnumValue(hkey, i++, value, &valsize, 0, &datatype, data, &datasize) == ERROR_SUCCESS)
		{
			// Get name of font
			string valstr = value;
			valstr = valstr.substr(0, valstr.find_first_of('(')-1);

			// Check if the value was found
			if(fontName == valstr)
			{
				// Set filename
				char *winDir = new char[MAX_PATH];
				GetWindowsDirectory(winDir, MAX_PATH);
				fileName = string(winDir) + "\\Fonts\\" + string(reinterpret_cast<char*>(data));

				// Clean up
				delete[] value;
				delete[] data;
				delete[] winDir;
				return true;
			}

			// Reset variables
			ZeroMemory(value, maxValue);
			ZeroMemory(data, maxData);
			valsize = maxValue;
			datasize = maxData;
		}

		// Clean up
		delete[] value;
		delete[] data;
	}
	return false;
}

// Finds next power of two
inline int next_p2(int a)
{
	int rval = 2;
	while(rval < a) rval <<= 1;
	return rval;
}

int OpenGLRender::loadFont(const string &fontPathOrName, const uint fontSize)
{
	// Check if we can find the file in the local directories
	string filePath;
	if(iosystem::isFile(fontPathOrName))
	{
		// Set file
		filePath = fontPathOrName;
	}else{
		// Loop throught the registry to find the file by font name
		if(!getFontFile(fontPathOrName.c_str(), filePath)) {
			ERR("Font '%s' not found!", filePath.c_str());
			return -1;
		}
	}

	// Create and initialize a FreeType library
	FT_Library library;
	if(FT_Init_FreeType(&library))
		assert("FT_Init_FreeType failed");

	// Load the font information from file
	FT_Face face;
	if(FT_New_Face(library, filePath.c_str(), 0, &face) != 0)
		assert("FT_New_Face failed (there is probably a problem with your font file)");

	// Allocate memory to store texture ids
	int id = m_nextFontId;
	Font *font = &Fonts[m_nextFontId++];
	font->filePath = filePath;
	font->fontSize = fontSize;

	// Generate texture and resize chars
	glGenTextures(1, &font->texture);
	font->chars.resize(128);

	// Get font name
	font->fontName = face->family_name + string(" ") + math::intToStr(fontSize);

	// Set character size
	FT_Set_Char_Size(face, fontSize << 6, fontSize << 6, 96, 96);

	// Load bitmap data for each of the character of the font
	RectPacker packer;
	for(uchar ch = 0; ch < 128; ch++)
	{
		// Load the glyph for our character
		if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
			assert("FT_Load_Glyph failed");

		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph glyph;
		if(FT_Get_Glyph(face->glyph, &glyph))
			assert("FT_Get_Glyph failed");

		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

		// This reference will make accessing the bitmap easier
		FT_Bitmap &bitmap = bitmapGlyph->bitmap;

		// Get dimentions of glyph
		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		// Allocate memory for the texture data
		uchar* charData = new uchar[1 + 2*width*height]; // Create a 16-bit buffer
		for(uint y = 0; y < height; y++)
		{ 
			for(uint x = 0; x < width; x++)
			{
				charData[2*(x + y*width)] = 255;
				charData[2*(x + y*width) + 1] = (x >= bitmap.width || bitmap.rows-y-1 >= bitmap.rows) ? 0 : bitmap.buffer[x + (bitmap.rows-y-1)*bitmap.width];// = 0;
			}
		}

		charData[2*width*height] = ch;

		// Get width of our font texture
		RectPacker::Rect rect(charData);
		rect.setSize(width, height);
		packer.addRect(rect);

		// Store advance
		font->chars[ch].advance = face->glyph->advance.x >> 6;
		font->chars[ch].pos.set(0, (-bitmapGlyph->top+(bitmap.rows-height)));

		// Finish the display list
		FT_Done_Glyph(glyph);
	}

	// Pack rects
	RectPacker::Result result = packer.pack();

	// Create font texture
	uchar *dataPtr = new uchar[result.canvas.x*result.canvas.y*2];
	memset(dataPtr, 0, result.canvas.x*result.canvas.y*2);
	for(uchar i = 0; i < 128; i++)
	{
		// Fill glyph data
		RectPacker::Rect rect = result.rectList[i];
		uchar *rectData = (uchar*)rect.getData();
		uchar ch = rectData[2*rect.getArea()];
		int dataPos = 0, rectPos = 0;
		for(int y = 0; y < rect.getHeight(); y++)
		{
			for(int x = 0; x < rect.getWidth(); x++)
			{
				dataPos = 2 * (((rect.getY()+y) * result.canvas.x) + (rect.getX()+x));
				rectPos = 2 * (y*rect.getWidth()+x);
				dataPtr[dataPos] = rectData[rectPos];
				dataPtr[dataPos+1] = rectData[rectPos+1];
			}
		}
		delete[] rectData;

		// Set char tex coord
		font->chars[ch].size = rect.getSize();
		font->chars[ch].texCoord1.set((float)rect.getRight()/(float)result.canvas.x, (float)rect.getBottom()/(float)result.canvas.y);// = rect.getSize()/result.canvas;
		font->chars[ch].texCoord0.set((float)rect.getLeft()/(float)result.canvas.x, (float)rect.getTop()/(float)result.canvas.y);
	}

	// Set texture param
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Fill texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result.canvas.x, result.canvas.y, 0,
		GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, (GLvoid*)dataPtr);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	delete[] dataPtr;

	// Setup texture
	Texture *texture = &Textures[font->texture];
	texture->width = result.canvas.x;
	texture->height = result.canvas.y;

	return id;
}

void OpenGLRender::setFont(const uint fontId)
{
	// Set current font
	if(Fonts.find(fontId) != Fonts.end())
		m_currentFont = &Fonts[fontId];
}

float OpenGLRender::stringWidth(const string &str)
{
	// Draw text using current font
	if(m_currentFont == 0)
	{
		ERR("OpenGLRender::stringWidth() No font is set!");
		return 0.0f;
	}

	// Create and initialize a FreeType library
	FT_Library library;
	if(FT_Init_FreeType(&library))
		assert("FT_Init_FreeType failed");

	// Load the font information from file
	FT_Face face;
	if(FT_New_Face(library, m_currentFont->filePath.c_str(), 0, &face))
		assert("FT_New_Face failed (there is probably a problem with your font file)");

	// Set character size
	FT_Set_Char_Size(face, 0, m_currentFont->fontSize << 6, 96, 96);

	FT_GlyphSlot slot = face->glyph;
	float width = 0.0f;
	for(uint i = 0; i < str.length(); i++)
	{
		FT_Error error = FT_Load_Char(face, str[i], FT_LOAD_DEFAULT);
		if(error) continue;
		width += slot->advance.x >> 6;
	}

	// Clean up FreeType
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return width;
}

float OpenGLRender::stringHeight(const string &str)
{
	float h = m_currentFont->fontSize/0.63f;
	int lines = 1;
	for(uint i = 0; i < str.length(); i++) {
		if(str[i] == '\n') lines++;
	}
	return h*lines;
}

void OpenGLRender::setDepth(const int depth)
{
	m_currentState.depth = depth;
}

void OpenGLRender::begin(const X2DDrawMode mode)
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

/*void printCallstack()
{
ScriptContext *ctx = asGetActiveContext();
for(uint i = 0; i < ctx->GetCallstackSize(); i++)
{
const char *scriptSection;
int line, col;
ScriptFunction *func = ctx->GetFunction(i);
line = ctx->GetLineNumber(i, &col, &scriptSection);
print("%s:%s:%d,%d", scriptSection, func->GetDeclaration(), line, col);
}
}*/

void OpenGLRender::addVertex(const Vector2 pos)
{
	// Make sure we have a buffer bound
	if(!m_currentBuffer)
		return;

	// Add vertex to batch
	m_currentVertex.position = pos;
	m_currentBuffer->addVertex(m_currentVertex);
}

void OpenGLRender::addRect(const Vector2 pos, const Vector2 size, const Vector2 coord0, const Vector2 coord1, const bool center)
{
	// Top-left
	if(center) {
		m_currentVertex.position = pos-size/2;
		m_currentVertex.texCoord.set(coord0.x, coord1.y);
	}else{
		m_currentVertex.position = pos;
		m_currentVertex.texCoord.set(coord0.x, coord1.y);
	}
	Vertex v0 = m_currentVertex;

	// Top-right
	m_currentVertex.position.x += size.x;
	m_currentVertex.texCoord.set(coord1.x, coord1.y);
	Vertex v1 = m_currentVertex;

	// Bottom-right
	m_currentVertex.position.y += size.y;
	m_currentVertex.texCoord.set(coord1.x, coord0.y);
	Vertex v2 = m_currentVertex;

	// Bottom-left
	m_currentVertex.position.x -= size.x;
	m_currentVertex.texCoord.set(coord0.x, coord0.y);
	Vertex v3 = m_currentVertex;

	m_currentBuffer->addVertex(v0);
	m_currentBuffer->addVertex(v3);
	m_currentBuffer->addVertex(v1);

	m_currentBuffer->addVertex(v1);
	m_currentBuffer->addVertex(v3);
	m_currentBuffer->addVertex(v2);
}

void OpenGLRender::end()
{
	m_currentBuffer = 0;
}

void OpenGLRender::beginClip(const Vector2i pos, const Vector2i size)
{
	rect2i rect(pos, size);

	// For some ODD reason, glScissor expects bottom-left coordinates
	rect.setY(m_currentOrtho[2]-rect.getBottom());

	m_currentState.clipRect = rect;
	m_clipStack.push_back(rect);
}

void OpenGLRender::endClip()
{
	m_clipStack.pop_back();
	m_currentState.clipRect = (m_clipStack.size() > 0) ? m_clipStack.back() : rect2i(0, 0, 0, 0);
}

void OpenGLRender::enableWireframe()
{
	// Enable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void OpenGLRender::disableWireframe()
{
	// Disable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLRender::setTexture(const int texId)
{
	if(texId < 0)
		return;

	// Update current draw state
	m_currentState.texture = texId;
}

void OpenGLRender::setShader(const int shaderId)
{
	// Set current shader program
	m_currentState.shader.program = shaderId;
	if(shaderId == 0)
	{
		m_currentState.shader.textureCount = 0;
		m_currentState.shader.uniforms.clear();
	}
}

void OpenGLRender::setTexCoord(const Vector2 coord)
{
	m_currentVertex.texCoord = coord;
}

void OpenGLRender::setColor(const vec4 color)
{
	m_currentVertex.color = color;
}

void OpenGLRender::setBlendFunc(const X2DBlendFunc srcc, const X2DBlendFunc dstc, const X2DBlendFunc srca, const X2DBlendFunc dsta)
{
	m_currentState.srcBlendColor = toGLBlend(srcc);
	m_currentState.dstBlendColor = toGLBlend(dstc);
	m_currentState.srcBlendAlpha = toGLBlend(srca);
	m_currentState.dstBlendAlpha = toGLBlend(dsta);
}

void OpenGLRender::reset(const uint bits)
{
	if(bits & X2D_GfxDepth)
		m_currentState.depth = 0;
	if(bits & X2D_GfxColor)
		m_currentVertex.color = vec4(1.0f);
	if(bits & X2D_GfxTexture)
		m_currentState.texture = 0;
	if(bits & X2D_GfxBlendFunc) {
		m_currentState.srcBlendColor = GL_SRC_ALPHA;
		m_currentState.dstBlendColor = GL_ONE_MINUS_SRC_ALPHA;
		m_currentState.srcBlendAlpha = GL_SRC_ALPHA;
		m_currentState.dstBlendAlpha = GL_ONE_MINUS_SRC_ALPHA;
	}
	if(bits & X2D_GfxShader)
		setShader(0);
}

void OpenGLRender::pushTransform(const Vector2 t, const vec4 r, const Vector2 s)
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

void OpenGLRender::popTransform()
{
	//if(m_matrixStack.size() == 1)
	//	error("Cannot pop the identity matrix");
	//m_matrixStack.pop_back();
	glPopMatrix();
}

const int floatSize = sizeof(float);

void OpenGLRender::drawFrontBatch()
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, state->texture);

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

		// Apply clipping
		if(state->clipRect.getArea() != 0)
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(state->clipRect.getX(), state->clipRect.getY(), state->clipRect.getWidth(), state->clipRect.getHeight());
		}

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

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, state->vbo);

			glVertexPointer(2, GL_FLOAT, 8*floatSize, (void*)(0*floatSize));
			glColorPointer(4, GL_FLOAT, 8*floatSize, (void*)(2*floatSize));
			glTexCoordPointer(2, GL_FLOAT, 8*floatSize, (void*)(6*floatSize));

			glDrawArrays(vbo->drawMode, vbo->begin, vbo->count);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

			glLoadIdentity();
		}
		delete buffer;
		glDisable(GL_SCISSOR_TEST);
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

int OpenGLRender::createTexture(const int width, const int height, const X2DTextureFilter filter, const bool mipmap)
{
	// Generate texture
	uint texid;
	glGenTextures(1, &texid);

	// Setup texture
	Texture *texture = &Textures[texid];
	texture->width = width;
	texture->height = height;

	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, (GLvoid*)vector<uint>(width*height, 0).data());
	if(mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter) == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilterMode(filter));
	glBindTexture(GL_TEXTURE_2D, 0);

	// Return tex id
	return texid;
}

void OpenGLRender::deleteTexture(const int texId)
{
	// Make sure it is a valid id
	if(Textures.find(texId) == Textures.end())
		return;

	// Delete the texture
	glDeleteTextures(1, (GLuint*)texId);
	Textures.erase(texId);
}

void OpenGLRender::setTextureFilter(const int texId, const X2DTextureFilter filter, const bool mipmap)
{
	glBindTexture(GL_TEXTURE_2D, texId);
	if(mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter) == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilterMode(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilterMode(filter));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRender::setTextureData(const int texId, const int width, const int height, Array &data, const bool mipmap)
{
	if(data.GetSize() != uint(width*height))
	{
		ERR("gfxSetTextureData(): Invalid data size (%i != %i)", data.GetSize(), width*height);
		return;
	}

	// Set texture data
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, (GLvoid*)data.At(0)); // GL_UNSIGNED_INT_8_8_8_8 for big-endian
	if(mipmap) glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set dimentions
	Textures[texId].width = width;
	Textures[texId].height = height;
}

Array *OpenGLRender::getTextureData(const int texId)
{
	// Create the array object
	Texture *texture = &Textures[texId];
	Array *arr = CreateArray("uint", texture->width*texture->height);

	// Get texture data
	uint *data = new uint[texture->width*texture->height];
	glBindTexture(GL_TEXTURE_2D, texId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to script array
	for(uint y = 0; y < texture->height; y++)
	{
		for(uint x = 0; x < texture->width; x++)
		{
			int i = x+y*texture->width;
			arr->SetValue(i, &data[i]);
		}
	}
	return arr;
}

int OpenGLRender::loadShader(const string &vertFilePath, const string &fragFilePath)
{
	// Create vertex and fragment shaders
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shader files to string
	const char *vertShaderBuffer, *fragShaderBuffer;
	long vertBufferLen = 0, fragBufferLen = 0; 
	m_assetLoader->loadAsset(vertFilePath.c_str(), &vertShaderBuffer, &vertBufferLen);
	m_assetLoader->loadAsset(fragFilePath.c_str(), &fragShaderBuffer, &fragBufferLen);

	// Default vars
	int result = 0;
	int logLength;

	// Compile vertex shader
	LOG("Compiling vertex shader: %s", vertFilePath.c_str());
	glShaderSource(vertShader, 1, &vertShaderBuffer, (int*)&vertBufferLen);
	glCompileShader(vertShader);
	delete[] vertShaderBuffer;

	// Validate vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
	char* compileLog = new char[logLength];
	glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
		LOG("\tCompile error: %s", compileLog);

	// Compile fragment shader
	LOG("Compiling fragment shader: %s", fragFilePath.c_str());
	glShaderSource(fragShader, 1, &fragShaderBuffer, (int*)&fragBufferLen);
	glCompileShader(fragShader);
	delete[] fragShaderBuffer;

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

	// Get error log
	glGetShaderInfoLog(vertShader, logLength, NULL, compileLog);

	// Print shader error to console
	if(logLength > 1)
		LOG("\tCompile error: %s", compileLog);

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
		LOG("\tCompile error: %s", compileLog);

	// Delete shader buffers as they are loaded into the shader program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Delete log buffers
	delete[] compileLog;
	delete[] programLog;

	// Return shader program
	return program;
}

void OpenGLRender::setUniform1i(const string &name, const int v0)
{
	// Set uniform
	Uniform uniform(Uniform1i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[1];
	((int*)uniform.data)[0] = v0;
	m_currentState.shader.uniforms.push_back(uniform);
}

void OpenGLRender::setUniform2i(const string &name, const int v0, const int v1)
{
	// Set uniform
	Uniform uniform(Uniform2i);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new int[2];
	((int*)uniform.data)[0] = v0;
	((int*)uniform.data)[1] = v1;
	m_currentState.shader.uniforms.push_back(uniform);
}

void OpenGLRender::setUniform3i(const string &name, const int v0, const int v1, const int v2)
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

void OpenGLRender::setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3)
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

void OpenGLRender::setUniform1f(const string &name, const float v0)
{
	// Set uniform
	Uniform uniform(Uniform1f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[1];
	((float*)uniform.data)[0] = v0;
	m_currentState.shader.uniforms.push_back(uniform);
}

void OpenGLRender::setUniform2f(const string &name, const float v0, const float v1)
{
	// Set uniform
	Uniform uniform(Uniform2f);
	uniform.loc = glGetUniformLocation(m_currentState.shader.program, name.c_str());
	uniform.data = (void*)new float[2];
	((float*)uniform.data)[0] = v0;
	((float*)uniform.data)[1] = v1;
	m_currentState.shader.uniforms.push_back(uniform);
}

void OpenGLRender::setUniform3f(const string &name, const float v0, const float v1, const float v2)
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

void OpenGLRender::setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3)
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

void OpenGLRender::setUniformTexture(const string &name, const int texId)
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

int OpenGLRender::loadImage(const string &filePath, const X2DTextureFilter filter, const bool mipmap)
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

Array *OpenGLRender::loadImageData(const string &filePath, int &w, int &h)
{
	// Create script array
	Array *arr = CreateArray("uint", 0);

	if(!iosystem::isFile(filePath))
	{
		ERR("gfxLoadImageData() file '%s' does not exist", filePath.c_str());
		return arr;
	}

	// Load image data
	uint *data = 0; uint width = 0, height = 0;
	int e = 0;
	if((e = m_assetLoader->loadAssetAsImage(filePath.c_str(), &data, width, height)) != X2D_OK)
	{
		ERR("Unable to load image with error code '%i'", e);
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

void OpenGLRender::saveImageData(Array *data, const string &filePath, const int w, const int h)
{
}

void OpenGLRender::clearTexture(const int texId)
{
	// Blank pixel data
	Texture *texture = &Textures[texId];

	// Clear texture
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, vector<uint>(texture->width*texture->height, 0).data());
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glClearTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, &clearColor); // The better way, although requires OpenGL 4.3
}

void OpenGLRender::exportTexture(const int textureId, const string &filePath)
{
	if(Textures.find(textureId) == Textures.end())
		return;

	// Write bitmap
	Texture *texture = &Textures[textureId];

	// Get texture data
	uint *data = new uint[texture->width*texture->height];
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_assetLoader->saveImage(filePath.c_str(), data, texture->width, texture->height);
}

void OpenGLRender::getTextureSize(const int texId, int &w, int &h)
{
	// Check for bound texture
	if(Textures.find(texId) == Textures.end())
		return;

	// Get texture
	w = Textures[texId].width;
	h = Textures[texId].height;
}

int OpenGLRender::copyToTexture(const int texId, const int x, const int y, const int width, const int height)
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

int OpenGLRender::createFrameBuffer()
{
	GLuint id;
	glGenFramebuffers(1, &id);
	return id;
}

void OpenGLRender::bindFrameBuffer(const int fboId)
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

void OpenGLRender::attachTextureToFrameBuffer(const int textureId)
{
	// Attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
}

void OpenGLRender::clearFrameBuffer()
{
	// Clear the color
	glClear(GL_COLOR_BUFFER_BIT);
}

int OpenGLRender::createVertexBuffer()
{
	GLuint id;
	glGenBuffersARB(1, &id);
	VertexBuffers.push_back(id);
	return id;
}

void OpenGLRender::bindVertexBuffer(const int vboId)
{
	// Bind frame buffer
	m_currentState.vbo = vboId;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
}

void OpenGLRender::setVertexBufferData(const Array &data, const X2DVertexBufferMode mode)
{
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, data.GetSize()*sizeof(float), data.At(0), toGLBufferMode(mode));
}

void OpenGLRender::setVertexBufferSubData(const int offset, const Array &data)
{
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset*sizeof(float), data.GetSize()*sizeof(float), data.At(0));
}

void OpenGLRender::drawVertexBuffer(const X2DDrawMode drawMode, const int begin, const int count)
{
	// Make sure no buffer is already bound
	if(m_currentBuffer)
		return;

	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	// Allocate static buffer
	StaticBuffer *buffer = new StaticBuffer;
	buffer->drawMode = toGLDrawMode(drawMode);
	buffer->begin = begin;
	buffer->count = count;
	memcpy(buffer->transform, m, 16*floatSize);
	m_batchStack.back().buffers[m_currentState] = buffer;
}

int OpenGLRender::getDrawCallCount()
{
	if(m_batchStack.size() == 0) return 0;
	return m_batchStack.back().buffers.size();
}  
#endif // OLD
