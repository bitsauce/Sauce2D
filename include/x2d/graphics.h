#ifndef X2D_GRAPHICS_H
#define X2D_GRAPHICS_H

#include <x2d/config.h>
#include <x2d/base.h>
#include <x2d/util.h>
#include <x2d/console.h>

class Pixmap;
class Texture;
class Shader;
class Batch;
class Sprite;
class TextureRegion;
class Vector2;
class VertexBufferObject;

extern uint QUAD_INDICES[6];
extern Vector4 QUAD_VERTICES[4];
extern Vector2 QUAD_TEXCOORD[4];

/*********************************************************************
**	Abstract Graphics Layer											**
**********************************************************************/

class XDAPI xdGraphics
{
	friend class xdEngine;
	friend class Batch;
public:
	AS_DECL_SINGLETON

	xdGraphics();

	// Global factory
	static Texture *CreateTexture(const Pixmap &pixmap);
	static Texture *CreateTexture(const string &filePath);
	static Texture *CreateTexture(const int width, const int height);
	static Texture *CreateTexture(const Texture &texture);
	static Shader *CreateShader(const string &vertFilePath, const string &fragFilePath);
	static Sprite *CreateSprite(const TextureRegion *, const bool);
	static TextureRegion *CreateTextureRegion(Texture *texture);
	static TextureRegion *CreateTextureRegion(Texture *texture, const Vector2 &uv0, const Vector2 &uv1);
	static TextureRegion *CreateTextureRegion(Texture *texture, const float u0, const float v0, const float u1, const float v1);
	static VertexBufferObject *CreateVertexBufferObject();

	// Refresh rate
	void setRefreshRate(const int hz);
	int getRefreshRate() const;

	// Vsync
	virtual void enableVsync()										{ NOT_IMPLEMENTED(enableVsync) }
	virtual void disableVsync()										{ NOT_IMPLEMENTED(disableVsync) }

	//
	float getTimeStep() const;
	float getFPS() const;

	// Swap buffers
	virtual void swapBuffers() = 0;

private:
	float m_framesPerSecond;
	int m_refreshRate;
	float m_timeStep;

	virtual void renderBatch(const Batch &batch) = 0;
	virtual Texture *createTexture(const Pixmap &pixmap) = 0;
	virtual Shader *createShader(const string &vertFilePath, const string &fragFilePath) = 0;
	virtual VertexBufferObject *createVertexBufferObject() = 0;
	static xdGraphics *s_this;
};


/*
// gfx.h
unsigned int rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return r + (g << 8) + (b << 16) + (a << 24);
}

class XDAPI Graphicsx
{
	// Texture struct
	struct Texture {
		int width;
		int height;
	};

	// Vertex struct
	struct Vertex {
		Vertex() :
			position(0.0f),
			color(1.0f),
			texCoord(0.0f)
		{
		}
			
		// NOTE TO SELF: DO NOT ALTER ORDER OF DEFINITION
		Vector2 position;
		Vector4 color;
		Vector2 texCoord;
	};

	// Shader uniforms
	enum UniformType {
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
	
	// Uniform struct
	struct Uniform {
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

	// Current rendering state
	struct State {
		State() :
			depth(0),
			texture(0),
			drawMode(X2D_DrawTriangles),
			vbo(0),
			shader(),
			srcBlendColor(X2D_SrcAlphaBlend),
			dstBlendColor(X2D_OneMinusSrcAlphaBlend),
			srcBlendAlpha(X2D_SrcAlphaBlend),
			dstBlendAlpha(X2D_OneMinusSrcAlphaBlend)
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
		int texture;

		// Draw mode
		X2DDrawMode drawMode;

		// Current bound vbo
		int vbo;

		// Clipping rect
		Recti clipRect;

		// Current shader state
		ShaderState shader;

		// Current blend mode
		X2DBlendFunc srcBlendColor;
		X2DBlendFunc dstBlendColor;
		X2DBlendFunc srcBlendAlpha;
		X2DBlendFunc dstBlendAlpha;
	};

	// Draw buffer types
	enum BufferType {
		DynamicBufferType,
		StaticBufferType
	};
	
	// Draw buffer
	struct DrawBuffer {
		DrawBuffer(BufferType type) :
			type(type)
		{
		}

		virtual ~DrawBuffer()
		{
		}

		BufferType type;
	};

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
			Matrix4 modelView(m);
			Vector4 newpos = Vector4(v0.position.x, v0.position.y, 0.0f, 1.0f)*modelView; // TDOD: Speed up by using my own Vector4
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

	// Static buffers (vbos)
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
	
	// Draw batches
	struct Batch {
		map<State, DrawBuffer*> buffers;
	};
	
	map<int, Texture> Textures;
	
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
		Vector2 pos;
		Vector2 size;
		Vector2 texCoord0;
		Vector2 texCoord1;
	};
	
	struct Font {
		string filePath;
		string fontName;
		uint fontSize;
		uint texture;
		vector<Char> chars;
	};
	
	// TODO: Move to X2DRender
	map<int, Font> Fonts;
	
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
	vector<Matrix4> m_matrixStack;

	// Engine handle
	xdEngine *m_engine;

	// Asset loader
	AssetLoader *m_assetLoader;
};


// Blend enum
enum X2DBlendFunc
{
	X2D_ZeroBlend,
	X2D_OneBlend,
	X2D_SrcColorBlend,
	X2D_OneMinusSrcColorBlend,
	X2D_SrcAlphaBlend,
	X2D_OneMinusSrcAlphaBlend,
	X2D_DstAlphaBlend,
	X2D_OneMinusDstAlphaBlend,
	X2D_DstColorBlend,
	X2D_OneMinusDstColorBlend,
	X2D_SrcAlphaSaturate
};

// Draw mode enum
enum X2DDrawMode
{
	X2D_DrawPoints,
	X2D_DrawLines,
	X2D_DrawLineLoop,
	X2D_DrawTriangles,
	X2D_DrawTriangleStrip,
	X2D_DrawTriangleFan
};
	
// Vertex buffer mode enum
enum X2DVertexBufferMode
{
	X2D_StreamBuffer,
	X2D_StaticBuffer,
	X2D_DynamicBuffer
};

// Texture filter enum
enum X2DTextureFilter
{
	X2D_NearestFilter,
	X2D_BilinearFilter
};

enum X2DStateBit
{
	X2D_GfxDepth		=	1 << 0,
	X2D_GfxColor		=	1 << 1,
	X2D_GfxTexture		=	1 << 2,
	X2D_GfxBlendFunc	=	1 << 3,
	X2D_GfxShader		=	1 << 4
};
*/

//XDAPI uint rgb(uchar r, uchar g, uchar b, uchar a = 255);

#endif // X2D_GRAPHICS_H