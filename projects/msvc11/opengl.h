#ifndef GFX_OPENGL_H
#define GFX_OPENGL_H

#include "x2d/gfx.h"
#include "x2d/math/matrix.h"

// Windows GL
#include "x2d/platform.h"
#include "x2d/math.h"
#include <GL/glew.h>
#include <GL/wglew.h>

class X2DEngine;

class Array;
class TextureAtlas;
class AssetLoader;


#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

/**
  *	\brief The OpenGLRender class is responsible for performing all the fundemental rendering operations.
  */
class OpenGLRender : public X2DRender
{
public:
	OpenGLRender(X2DEngine *engine, AssetLoader *assetLoader);
	~OpenGLRender();

	void init();
	void beginDraw();
	void endDraw();

	void createContext(HWND window);
	void destroyContext();

	// Font drawing
	// TODO: Move to X2DRender
	struct Char {
		Char() :
			advance(0.0f),
			pos(0),
			size(0),
			texCoord0(0),
			texCoord1(0)
		{
		}

		float advance;
		vec2 pos;
		vec2 size;
		vec2 texCoord0;
		vec2 texCoord1;
	};
	
	// TODO: Move to X2DRender
	struct Font {
		string filePath;
		string fontName;
		uint fontSize;
		uint texture;
		vector<Char> chars;
	};
	
	// TODO: Move to X2DRender
	map<int, Font> Fonts;

	// Viewport & projection
	void setViewport(const vec2i pos, const vec2i size);
	void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f);
	void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f);

	// Text drawing
	int loadFont(const string &fontPathOrName, const uint fontSize);
	void setFont(const uint fontId);
	float stringWidth(const string &str);
	float stringHeight(const string &str);

	// State operations
	void setDepth(const int depth);
	void setTexture(const int texture);
	void setTexCoord(const vec2 coord);
	void setBlendFunc(const X2DBlendFunc srcc, const X2DBlendFunc dstc, const X2DBlendFunc srca, const X2DBlendFunc dsta);
	void setColor(const vec4 color);
	void setShader(const int shader);
	void reset(const uint bits);

	// Primitive drawing
	void begin(const X2DDrawMode drawMode = X2D_DrawTriangles);
	void end();
	void addText(const vec2 pos, const string &text);
	void addRect(const vec2 pos, const vec2 size, const vec2 coord0 = vec2(0.0f), const vec2 coord1 = vec2(1.0f), const bool center = false);
	void addVertex(const vec2 pos);
	
	// Draw clipping
	void beginClip(const vec2i pos, const vec2i size);
	void endClip();
	vector<rect2i> m_clipStack;

	// Wireframe
	void enableWireframe();
	void disableWireframe();

	// Transformation
	void pushTransform(const vec2 translate, const vec4 rotation, const vec2 scale);
	void popTransform();

	// Vertext struct
	// TODO: Move to X2DRender
	struct Vertex {
		Vertex() :
			position(0.0f),
			color(1.0f),
			texCoord(0.0f)
		{
		}
			
		// NOTE: DO NOT ALTER ORDER OF DEFINITION
		vec2 position;
		vec4 color;
		vec2 texCoord;
	};
	
	// TODO: Move to X2DRender
	enum UniformType
	{
		Uniform1i,
		Uniform2i,
		Uniform3i,
		Uniform4i,
		Uniform1f,
		Uniform2f,
		Uniform3f,
		Uniform4f,
		UniformTexture
	};
	
	// TODO: Move to X2DRender
	struct Uniform
	{
		Uniform(const UniformType type) :
			type(type),
			loc(0),
			data(0)
		{
		}

		bool operator<(const Uniform& other) const
		{
			TUPLE_CMP(this->type, other.type)
			TUPLE_CMP(this->loc, other.loc)
			TUPLE_CMP(this->data, other.data)
			return false;
		}

		UniformType type;
		int loc;
		void *data;
	};
	
	// TODO: Move to X2DRender
	struct ShaderState
	{
		ShaderState() :
			program(0),
			textureCount(0)
		{
		}

		bool operator<(const ShaderState& other) const
		{
			TUPLE_CMP(this->program, other.program)
			TUPLE_CMP(this->textureCount, other.textureCount)
			TUPLE_CMP(this->uniforms, other.uniforms)
			return false;
		}

		bool operator>(const ShaderState& other) const
		{
			TUPLE_CMP2(this->program, other.program)
			TUPLE_CMP2(this->textureCount, other.textureCount)
			TUPLE_CMP2(this->uniforms, other.uniforms)
			return false;
		}

		uint program;
		uint textureCount;
		vector<Uniform> uniforms;
	};

	// This separates the different draw states
	// TODO: Move to X2DRender
	struct State {
		State() :
			depth(0),
			texture(0),
			drawMode(GL_QUADS),
			vbo(0),
			shader(),
			srcBlendColor(GL_SRC_ALPHA),
			dstBlendColor(GL_ONE_MINUS_SRC_ALPHA),
			srcBlendAlpha(GL_SRC_ALPHA),
			dstBlendAlpha(GL_ONE_MINUS_SRC_ALPHA)
		{
		}

		bool operator<(const State& other) const
		{
			// NOTE: The map is sorted by the order of compares
			TUPLE_CMP(this->depth, other.depth)
			TUPLE_CMP(this->texture, other.texture)
			TUPLE_CMP(this->drawMode, other.drawMode)
			TUPLE_CMP(this->vbo, other.vbo)
			TUPLE_CMP(this->clipRect, other.clipRect)
			// TODO: At this point it seems like bad idea to compare shaders due to potential overhead
			TUPLE_CMP(this->shader, other.shader)
			TUPLE_CMP(this->srcBlendColor, other.srcBlendColor)
			TUPLE_CMP(this->dstBlendColor, other.dstBlendColor)
			TUPLE_CMP(this->srcBlendAlpha, other.srcBlendAlpha)
			TUPLE_CMP(this->dstBlendAlpha, other.dstBlendAlpha)
			return false;
		}

		// Current draw depth (used when sorting)
		int depth;

		// Current bound texture
		uint texture;

		// Draw mode
		uint drawMode;

		// Current bound vbo
		uint vbo;

		// Clipping rect
		rect2i clipRect;

		// Current shader state
		ShaderState shader;

		// Current blend mode
		uint srcBlendColor;
		uint dstBlendColor;
		uint srcBlendAlpha;
		uint dstBlendAlpha;
	};

	// Batching buffer types
	// TODO: Move to X2DRender
	enum BufferType
	{
		DynamicBufferType,
		StaticBufferType
	};

	// Draw batch buffer
	// TODO: Move to X2DRender
	struct DrawBuffer
	{
		DrawBuffer(BufferType type) :
			type(type)
		{
		}

		virtual ~DrawBuffer()
		{
		}

		BufferType type;
	};

	// For storing dynamically allocated vertices
	// TODO: Move to X2DRender
	struct DynamicBuffer : public DrawBuffer
	{
		DynamicBuffer() :
			DrawBuffer(DynamicBufferType)
		{
		}

		virtual ~DynamicBuffer()
		{
		}

		void addVertex(Vertex v0)
		{
			// Apply modelview
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			mat4 modelView(m);
			vec4 newpos = vec4(v0.position.x, v0.position.y, 0.0f, 1.0f)*modelView; // TDOD: Speed up by using my own vec4
			v0.position.set(newpos.x, newpos.y); 

			vertices.push_back(v0);
		}

		void *data()
		{
			return vertices.data();
		}

		int vertexCount()
		{
			return vertices.size();
		}

	private:
		vector<Vertex> vertices;
	};

	// Predefined buffers (vbos)
	// TODO: Move to X2DRender
	struct StaticBuffer : public DrawBuffer
	{
		StaticBuffer() :
			DrawBuffer(StaticBufferType),
			drawMode(0),
			begin(0),
			count(0)
		{
		}

		virtual ~StaticBuffer()
		{
		}

		uint drawMode;
		uint begin;
		uint count;
		float transform[16];
	};
	
	// TODO: Move to X2DRender
	struct Batch
	{
		map<State, DrawBuffer*> buffers;
	};

	void drawFrontBatch();
	void clearBatches();
	
	// Texture struct
	// TODO: Move to X2DRender
	struct Texture {
		uint width;
		uint height;
	};

	// Texture functions
	map<int, Texture> Textures;
	int createTexture(const int w, const int h, const X2DTextureFilter filter = X2D_NearestFilter, const bool mipmap = false);
	void deleteTexture(const int texId);
	void setTextureFilter(const int texId, const X2DTextureFilter filter, const bool mipmap = false);
	void setTextureData(const int texId, const int width, const int height, Array &data, const bool mipmap = false);
	Array *getTextureData(const int texId);
	int copyToTexture(const int texId, const int x, const int y, const int width, const int height);
	void clearTexture(const int texId);
	void getTextureSize(const int texId, int &w, int &h);
	void exportTexture(const int textureId, const string &filePath);

	// Shader functions
	int loadShader(const string &vertFilePath, const string &fragFilePath);
	void setUniform1i(const string &name, const int v0);
	void setUniform2i(const string &name, const int v0, const int v1);
	void setUniform3i(const string &name, const int v0, const int v1, const int v2);
	void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3);
	void setUniform1f(const string &name, const float v0);
	void setUniform2f(const string &name, const float v0, const float v1);
	void setUniform3f(const string &name, const float v0, const float v1, const float v2);
	void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3);
	void setUniformTexture(const string &name, const int texId);
	
	// Image functions
	int loadImage(const string &filePath, const X2DTextureFilter filter = X2D_NearestFilter, const bool mipmap = false);
	Array *loadImageData(const string &filePath, int &w, int &h);
	void saveImageData(Array *data, const string &filePath, const int w, const int h);

	// Frame buffer functions
	int  createFrameBuffer();
	void bindFrameBuffer(const int fboId);
	void attachTextureToFrameBuffer(const int texturId);
	void clearFrameBuffer();

	// Vertex buffer functions
	vector<int> VertexBuffers;
	int createVertexBuffer();
	void bindVertexBuffer(const int vboId);
	void setVertexBufferData(const Array &data, const X2DVertexBufferMode usage);
	void setVertexBufferSubData(const int offset, const Array &data);
	void drawVertexBuffer(const X2DDrawMode drawMode, const int begin, const int count);

	int getDrawCallCount();

private:
	
	// The OpenGL context
    HGLRC m_openGLContext;

	// Window device context
	HDC m_deviceContext;

	// ID counters
	uint m_nextFontId;
	
	// Current bindings
	Font *m_currentFont;
	TextureAtlas *m_currentAtlas;
	vector<Batch> m_batchStack;
	float m_currentOrtho[6];
	int m_currentFbo;
	Vertex m_currentVertex;
	State m_currentState;
	// TODO: Move to X2DRender
	DynamicBuffer *m_currentBuffer;
	
	// TODO: Move matrix stack to X2DRender
	vector<mat4> m_matrixStack;

	// Engine handle
	X2DEngine *m_engine;

	// Asset loader
	AssetLoader *m_assetLoader;
};

#endif // GFX_OPENGL_H