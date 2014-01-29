#ifndef ANDROID_RENDER_H
#define ANDROID_RENDER_H

#include "x2d/gfx.h"
#include "android_engine.h"

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

class AndroidRender : public X2DRender
{
public:
	AndroidRender(AppManager *manager);
	~AndroidRender();

	int initGL();
	void destroyGL();

	void beginDraw();
	void endDraw();

	// Font drawing
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

	struct Font {
		string filePath;
		string fontName;
		uint fontSize;
		uint texture;
		vector<Char> chars;
	};

	map<int, Font> Fonts;

	// Viewport & projection
	void setViewport(const int x, const int y, const int w, const int h);
	void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f);
	void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f);
	void clearColorBit();

	// State operations
	void setDepth(const int depth);
	void setTexture(const int texture);
	void setTexCoord(const vec2 coord);
	void setBlendFunc(const X2DBlendFunc src, const X2DBlendFunc dst);
	void setBlendFuncSep(const X2DBlendFunc srcc, const X2DBlendFunc dstc, const X2DBlendFunc srca, const X2DBlendFunc dsta);
	void setColor(const vec4 color);
	void setShader(const int shader);
	void resetDepth();
	void resetColor();
	void resetTexture();
	void resetBlendFunc();
	void resetShader();
	void reset();

	// Primitive drawing
	void begin(const X2DDrawMode drawMode = X2D_DrawTriangles);
	void end();
	void addText(const vec2 pos, const string &text);
	void addText(const float x, const float y, const string &text);
	void addRect(const vec2 pos, const vec2 size, const vec2 coord0 = vec2(0.0f), const vec2 coord1 = vec2(1.0f), const bool center = false);
	void addRect(const float x, const float y, const float w, const float h, const float cx0, const float cx1, const float cy0, const float cy1, const bool center = false);
	void addVertex(const vec2 pos);
	void addVertex(const float x, const float y);
	
	// Draw clipping
	void beginClip(const float x, const float y, const float width, const float height);
	void endClip();

	// Transformation
	void pushTransform(const vec2 trans, const vec4 rot, const vec2 scale);
	void popTransform();

	// Vertext struct
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
	struct State {
		State() :
			depth(0),
			texture(0),
			drawMode(GL_TRIANGLES),
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

		// Current shader state
		ShaderState shader;

		// Current blend mode
		uint srcBlendColor;
		uint dstBlendColor;
		uint srcBlendAlpha;
		uint dstBlendAlpha;
	};

	// Batching buffer types
	enum BufferType
	{
		DynamicBufferType,
		StaticBufferType
	};

	// Draw batch buffer
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
			vec4 newpos = vec4(v0.position.x, v0.position.y, 0.0f, 1.0f)*mat4(m); // TDOD: Speed up by implementing vec4.xy
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

	struct Batch
	{
		map<State, DrawBuffer*> buffers;
	};

	void drawFrontBatch();
	void clearBatches();
	
	// Texture struct
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
	int copyToTexture(const int texId, const int x, const int y, const int width, const int height);
	void clearTexture(const int texId);
	void getTextureSize(const int texId, int &w, int &h);

	// Shader functions
	// TO DOCUMENT ->
#ifdef USE_GLES_2_0
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
#endif
	// TO DOCUMENT <-
	
	// Image functions
	int loadImage(const string &filePath, const X2DTextureFilter filter = X2D_NearestFilter, const bool mipmap = false);
	Array *loadImageData(const string &filePath, int &w, int &h);

	// Frame buffer functions
#ifdef USE_GLES_2_0
	int  createFrameBuffer();
	void bindFrameBuffer(const int fboId);
	void attachTextureToFrameBuffer(const int texturId);
#endif

	// Vertex buffer functions
	vector<int> VertexBuffers;
	int createVertexBuffer();

	// TO DOCUMENT ->
	/**
	  * This function binds a vertex buffer object for manipulation
	  */
	void bindVertexBuffer(const int vboId);

	void setVertexBufferData(const Array &data, const X2DVertexBufferMode usage);
	void setVertexBufferSubData(const int offset, const Array &data);

	void drawVertexBuffer(const X2DDrawMode drawMode, const int begin, const int count);
	// TO DOCUMENT <-

	int getDrawCallCount();

private:
	
	// App manager
	AppManager *m_manager;

	// ID counters
	uint m_nextFontId;
	
	// Current bindings
	Font *m_currentFont;
	vector<Batch> m_batchStack;
	float m_currentOrtho[6];
	int m_currentFbo;
	Vertex m_currentVertex;
	State m_currentState;
	DynamicBuffer *m_currentBuffer;
};


#endif // ANDROID_RENDER_H