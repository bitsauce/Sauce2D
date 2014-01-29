#ifndef X2D_GFX_H
#define X2D_GFX_H

#include "platform.h"
#include "math.h"

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

/*********************************************************************
**	Abstract Graphics Render										**
**********************************************************************/
class X2DAPI X2DRender
{
public:
	
	// Render functions
	virtual void beginDraw() { NOT_IMPLEMENTED(beginDraw) }
	virtual void endDraw() { NOT_IMPLEMENTED(endDraw) }
	
	// Viewport & projection
	virtual void setViewport(const vec2i pos, const vec2i size) { NOT_IMPLEMENTED(setViewport) }
	virtual void setOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f) { NOT_IMPLEMENTED(setOrthoProjection) }
	virtual void getOrthoProjection(float &l, float &r, float &b, float &t, float &n, float &f) { NOT_IMPLEMENTED(getOrthoProjection) }
	
	// Text drawing
	virtual int loadFont(const string &fontPathOrName, const uint fontSize) { NOT_IMPLEMENTED_RET(loadFont, -1) }
	virtual void setFont(const uint fontId) { NOT_IMPLEMENTED(setFont) }
	virtual float stringWidth(const string &str) { NOT_IMPLEMENTED_RET(stringWidth, 0.0f) }
	virtual float stringHeight(const string &str) { NOT_IMPLEMENTED_RET(stringHeight, 0.0f) }
	
	// State operations
	virtual void setClearColor(const vec4 color) { NOT_IMPLEMENTED(setClearColor) }
	virtual void setDepth(const int depth) { NOT_IMPLEMENTED(setDepth) }
	virtual void setTexture(const int texture) { NOT_IMPLEMENTED(setTexture) }
	virtual void setTexCoord(const vec2 coord) { NOT_IMPLEMENTED(setTexCoord) }
	virtual void setBlendFunc(const X2DBlendFunc srcc, const X2DBlendFunc dstc, const X2DBlendFunc srca, const X2DBlendFunc dsta) { NOT_IMPLEMENTED(setBlendFuncSep) }
	virtual void setColor(const vec4 color) { NOT_IMPLEMENTED(setColor) }
	virtual void setShader(const int shader) { NOT_IMPLEMENTED(setShader) }
	virtual void reset(const uint bits) { NOT_IMPLEMENTED(reset) }
	
	// Primitive drawing
	virtual void begin(const X2DDrawMode drawMode) { NOT_IMPLEMENTED(begin) }
	virtual void end() { NOT_IMPLEMENTED(end) }
	virtual void addText(const vec2 pos, const string &text) { NOT_IMPLEMENTED(addText) }
	virtual void addRect(const vec2 pos, const vec2 size, const vec2 coord0, const vec2 coord1, const bool center) { NOT_IMPLEMENTED(addRect) }
	virtual void addVertex(const vec2 pos) { NOT_IMPLEMENTED(addVertex) }
	virtual int getDrawCallCount() { NOT_IMPLEMENTED_RET(getDrawCallCount, -1) }
	
	// Draw clipping
	virtual void beginClip(const vec2i pos, const vec2i size) { NOT_IMPLEMENTED(beginClip) }
	virtual void endClip() { NOT_IMPLEMENTED(endClip) }

	// Wireframe
	virtual void enableWireframe() { NOT_IMPLEMENTED(enableWireframe) }
	virtual void disableWireframe() { NOT_IMPLEMENTED(disableWireframe) }

	// Transformation
	virtual void pushTransform(const vec2 translate, const vec4 rotation, const vec2 scale) { NOT_IMPLEMENTED(pushTransform) }
	virtual void popTransform() { NOT_IMPLEMENTED(popTransform) }

	// Texture functions
	virtual int createTexture(const int w, const int h, const X2DTextureFilter filter, const bool mipmap) { NOT_IMPLEMENTED_RET(createTexture, -1) }
	virtual void deleteTexture(const int texId) { NOT_IMPLEMENTED(deleteTexture) }
	virtual void setTextureFilter(const int texId, const X2DTextureFilter filter, const bool mipmap) { NOT_IMPLEMENTED(setTextureFilter) }
	virtual void setTextureData(const int texId, const int width, const int height, Array &data, const bool mipmap) { NOT_IMPLEMENTED(setTextureData) }
	virtual Array *getTextureData(const int texId) { NOT_IMPLEMENTED_ARR(getTextureData, "vec4") }
	virtual int copyToTexture(const int texId, const int x, const int y, const int width, const int height) { NOT_IMPLEMENTED_RET(copyToTexture, -1) }
	virtual void clearTexture(const int texId) { NOT_IMPLEMENTED(clearTexture) }
	virtual void getTextureSize(const int texId, int &w, int &h) { NOT_IMPLEMENTED(getTextureSize) }
	virtual void exportTexture(const int textureId, const string &filePath) { NOT_IMPLEMENTED(exportTexture) }

	// Shader functions
	virtual int loadShader(const string &vertFilePath, const string &fragFilePath) { NOT_IMPLEMENTED_RET(loadShader, -1) }
	virtual void setUniform1i(const string &name, const int v0) { NOT_IMPLEMENTED(setUniform1i) }
	virtual void setUniform2i(const string &name, const int v0, const int v1) { NOT_IMPLEMENTED(setUniform2i) }
	virtual void setUniform3i(const string &name, const int v0, const int v1, const int v2) { NOT_IMPLEMENTED(setUniform3i) }
	virtual void setUniform4i(const string &name, const int v0, const int v1, const int v2, const int v3) { NOT_IMPLEMENTED(setUniform4i) }
	virtual void setUniform1f(const string &name, const float v0) { NOT_IMPLEMENTED(setUniform1f) }
	virtual void setUniform2f(const string &name, const float v0, const float v1) { NOT_IMPLEMENTED(setUniform2f) }
	virtual void setUniform3f(const string &name, const float v0, const float v1, const float v2) { NOT_IMPLEMENTED(setUniform3f) }
	virtual void setUniform4f(const string &name, const float v0, const float v1, const float v2, const float v3) { NOT_IMPLEMENTED(setUniform4f) }
	virtual void setUniformTexture(const string &name, const int texId) { NOT_IMPLEMENTED(setUniformTexture) }
	
	// Image functions
	virtual int loadImage(const string &filePath, const X2DTextureFilter filter, const bool mipmap) { NOT_IMPLEMENTED_RET(loadImage, -1) }
	virtual Array *loadImageData(const string &filePath, int &w, int &h) { NOT_IMPLEMENTED_ARR(loadImageData, "vec4") }
	virtual void saveImageData(Array *data, const string &filePath, const int w, const int h) { NOT_IMPLEMENTED(saveImageData) }

	// Texture atlas
	/*virtual int createTextureAtlas() { NOT_IMPLEMENTED_RET(createTextureAtlas, -1) }
	virtual void bindTextureAtlas(int atlasId) { NOT_IMPLEMENTED(bindTextureAtlas) }

	virtual int  addImageToAtlas(const string &filePath) { NOT_IMPLEMENTED_RET(addImageToAtlas, -1) }
	virtual int  addTextureToAtlas(const int textureId) { NOT_IMPLEMENTED_RET(addTextureToAtlas, -1) }
	virtual void getAtlasTex(const float &tx, const float &ty) { NOT_IMPLEMENTED(getAtlasTex) }

	virtual int getAtlasCount() { NOT_IMPLEMENTED_RET(getAtlasCount, -1) }
	virtual int getAtlasTextureByIndex(const int idx) { NOT_IMPLEMENTED_RET(getAtlasTextureByIndex, -1) }*/

	// Frame buffer functions
	virtual int  createFrameBuffer() { NOT_IMPLEMENTED_RET(createFrameBuffer, -1) }
	virtual void bindFrameBuffer(const int fboId) { NOT_IMPLEMENTED(bindFrameBuffer) }
	virtual void attachTextureToFrameBuffer(const int texturId) { NOT_IMPLEMENTED(attachTextureToFrameBuffer) }
	virtual void clearFrameBuffer() { NOT_IMPLEMENTED(clearFrameBuffer) }

	// Vertex buffer functions
	virtual int createVertexBuffer() { NOT_IMPLEMENTED_RET(createVertexBuffer, -1) }
	virtual void bindVertexBuffer(const int vboId) { NOT_IMPLEMENTED(bindVertexBuffer) }
	virtual void setVertexBufferData(const Array &data, const X2DVertexBufferMode mode) { NOT_IMPLEMENTED(setVertexBufferData) }
	virtual void setVertexBufferSubData(const int offset, const Array &data) { NOT_IMPLEMENTED(setVertexBufferSubData) }
	virtual void drawVertexBuffer(const X2DDrawMode drawMode, const int begin, const int count) { NOT_IMPLEMENTED(drawVertexBuffer) }
	
	// Overloads
	void setColor(const float r, const float g, const float b, const float a) { setColor(vec4(r, g, b, a)); }
	void setTexCoord(const float x, const float y) { setTexCoord(vec2(x, y)); }
	void addText(const float x, const float y, const string &text) { addText(vec2(x, y), text); }
	void addRect(const float x, const float y, const float w, const float h, const float cx0, const float cy0, const float cx1, const float cy1, const bool center) { addRect(vec2(x, y), vec2(w, h), vec2(cx0, cy0), vec2(cx1, cy1), center); }
	void addVertex(const float x, const float y) { addVertex(vec2(x, y)); }
	void setViewport(const int x, const int y, const int w, const int h) { setViewport(vec2i(x, y), vec2i(w, h)); }
	void setClearColor(const float r, const float g, const float b, const float a) { setClearColor(vec4(r, g, b, a)); }
	void setBlendFunc(const X2DBlendFunc src, const X2DBlendFunc dst) { setBlendFunc(src, dst, src, dst); }
	void beginClip(const float x, const float y, const float w, const float h) { beginClip(vec2(x, y), vec2(w, h)); }
	void pushTransform(const float px, const float py, const float ang, const float rx, const float ry, const float rz, const float sx, const float sy) { pushTransform(vec2(px, py), vec4(rx, ry, rz, ang), vec2(sx, sy)); }
	void resetDepth()		{ reset(X2D_GfxDepth); }
	void resetColor()		{ reset(X2D_GfxColor); }
	void resetTexture()		{ reset(X2D_GfxTexture); }
	void resetBlendFunc()	{ reset(X2D_GfxBlendFunc); }
	void resetShader()		{ reset(X2D_GfxShader); }
};

X2DAPI uint rgb(uchar r, uchar g, uchar b, uchar a = 255);

#endif // X2D_GFX_H