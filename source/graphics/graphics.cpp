//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/graphics.h>
#include <x2d/math.h>
#include <x2d/assetloader.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics/textureRegion.h>
#include <x2d/graphics/pixmap.h>

xdGraphics *xdGraphics::s_this = 0;

uint QUAD_INDICES[6] = { 0, 3, 1, 1, 3, 2 };
Vector4 QUAD_VERTICES[4] = {
	Vector4(0.0f, 0.0f, 0.0f, 1.0f),
	Vector4(1.0f, 0.0f, 0.0f, 1.0f),
	Vector4(1.0f, 1.0f, 0.0f, 1.0f),
	Vector4(0.0f, 1.0f, 0.0f, 1.0f)
};
Vector2 QUAD_TEXCOORD[4] = {
	Vector2(0.0f, 1.0f),
	Vector2(1.0f, 1.0f),
	Vector2(1.0f, 0.0f),
	Vector2(0.0f, 0.0f)
};

AS_REG_SINGLETON(xdGraphics, "ScriptGraphics")

int xdGraphics::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Enums
	/*AS_ENUM_REGISTER("BlendFunc")
	AS_ENUM_VALUE("BlendFunc", "ZeroBlend", X2D_ZeroBlend)
	AS_ENUM_VALUE("BlendFunc", "OneBlend", X2D_OneBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcColorBlend", X2D_SrcColorBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusSrcColorBlend", X2D_OneMinusSrcColorBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcAlphaBlend", X2D_SrcAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusSrcAlphaBlend", X2D_OneMinusSrcAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "DstAlphaBlend", X2D_DstAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusDstAlphaBlend", X2D_OneMinusDstAlphaBlend)
	AS_ENUM_VALUE("BlendFunc", "DstColorBlend", X2D_DstColorBlend)
	AS_ENUM_VALUE("BlendFunc", "OneMinusDstColorBlend", X2D_OneMinusDstColorBlend)
	AS_ENUM_VALUE("BlendFunc", "SrcAlphaSaturate", X2D_SrcAlphaSaturate)
	
	AS_ENUM_REGISTER("DrawMode")
	AS_ENUM_VALUE("DrawMode", "DrawPoints", X2D_DrawPoints)
	AS_ENUM_VALUE("DrawMode", "DrawLines", X2D_DrawLines)
	AS_ENUM_VALUE("DrawMode", "DrawLineLoop", X2D_DrawLineLoop)
	AS_ENUM_VALUE("DrawMode", "DrawTriangles", X2D_DrawTriangles)
	AS_ENUM_VALUE("DrawMode", "DrawTriangleStrip", X2D_DrawTriangleStrip)
	AS_ENUM_VALUE("DrawMode", "DrawTriangleFan", X2D_DrawTriangleFan)
	
	AS_ENUM_REGISTER("VertexBufferMode")
	AS_ENUM_VALUE("VertexBufferMode", "StreamBuffer", X2D_StreamBuffer)
	AS_ENUM_VALUE("VertexBufferMode", "StaticBuffer", X2D_StaticBuffer)
	AS_ENUM_VALUE("VertexBufferMode", "DynamicBuffer", X2D_DynamicBuffer)
	
	AS_ENUM_REGISTER("TextureFilter")
	AS_ENUM_VALUE("TextureFilter", "NearestFilter", X2D_NearestFilter)
	AS_ENUM_VALUE("TextureFilter", "BilinearFilter", X2D_BilinearFilter)

	// Projection functions
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetViewport(const Vector2i pos, const Vector2i size)", setViewport, (const Vector2i, const Vector2i), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetViewport(const int x, const int y, const int w, const int h)", setViewport, (const int, const int, const int, const int), void, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetViewport()", gfxGetViewport, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetOrthoProjection(const float l, const float r, const float b, const float t, const float n, const float f)", setOrthoProjection, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetOrthoProjection(float &out l, float &out r, float &out b, float &out t, float &out n, float &out f)", getOrthoProjection, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxGetDrawCallCount()", getDrawCallCount, gfx)

	// State modifiers
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetFont(const uint font)", setFont, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetTexCoord(const float x, const float y)", setTexCoord, (const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetTexCoord(const Vector2)", setTexCoord, (const Vector2), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetColor(const Vector4 color)", setColor, (const Vector4), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetColor(const float, const float, const float, const float a = 1.0f)", setColor, (const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTexture(const int)", setTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetDepth(const int depth)", setDepth, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetBlendFunc(const BlendFunc, const BlendFunc)", setBlendFunc, (const X2DBlendFunc, const X2DBlendFunc), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxSetBlendFuncSep(const BlendFunc, const BlendFunc, const BlendFunc, const BlendFunc)", setBlendFunc, (const X2DBlendFunc, const X2DBlendFunc, const X2DBlendFunc, const X2DBlendFunc), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetShader(const int shader)", setShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetDepth()", resetDepth, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetColor()", resetColor, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetTexture()", resetTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetBlendFunc()", resetBlendFunc, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxResetShader()", resetShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxReset()", reset, gfx)

	// Font
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadFont(const string &in, const uint size)", loadFont, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearFont(const uint fontId)", clearFont, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "float gfxGetStringWidth(const string &in)", stringWidth, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "float gfxGetStringHeight(const string &in)", stringHeight, gfx)

	// Drawing functions
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBegin(const DrawMode = DrawTriangles)", begin, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEnd()", end, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddText(const Vector2 pos, const string &in)", addText, (const Vector2, const string&), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddText(const float x, const float y, const string &in)", addText, (const float, const float, const string&), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddVertex(const Vector2 pos)", addVertex, (const Vector2), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddVertex(const float x, const float y)", addVertex, (const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddRect(const Vector2 pos, const Vector2 size, const Vector2 coord0, const Vector2 coord1, const bool center = false)", addRect, (const Vector2, const Vector2, const Vector2, const Vector2, const bool), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxAddRect(const float, const float, const float, const float, const float, const float, const float, const float, const bool center = false)", addRect, (const float, const float, const float, const float, const float, const float, const float, const float, const bool), void, gfx)

	// Clipping
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxBeginClip(const float x, const float y, const float width, const float height)", beginClip, (const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEndClip()", endClip, gfx)

	// Wireframe
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxEnableWireframe()", enableWireframe, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDisableWireframe()", disableWireframe, gfx)

	// Transform
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxPushTransform(const float px, const float py,"
															"const float rx = 0.0f, const float ry = 0.0f, const float rz = 0.0f, const float ang = 0.0f,"
															"const float sx = 1.0f, const float sy = 1.0f)", pushTransform,
															(const float, const float, const float, const float, const float, const float, const float, const float), void, gfx)
	AS_SINGLETON_FUNCTIONPR(X2DRender, "void gfxPushTransform(const Vector2 pos, const Vector4 rot = Vector4(0.0f), const Vector2 scale = Vector2(1.0f))", pushTransform,
															(const Vector2, const Vector4, const Vector2), void, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxPopTransform()", popTransform, gfx)

	// Texture processing
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateTexture(const int w, const int h, const TextureFilter filter = NearestFilter, const bool mipmap = false)", createTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDeleteTexture(const int)", deleteTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTextureFilter(const int, const TextureFilter filter = NearestFilter, const bool mipmap = false)", setTextureFilter, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetTextureData(const int, const int, const int, const array<uint> &in, const bool mipmap = false)", setTextureData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearTexture(const int)", clearTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "array<uint> @gfxGetTextureData(const int)", getTextureData, gfx)
	//AS_SINGLETON_FUNCTION(X2DRender, "array<uint> gfxReadPixels(const int x, const int y, const int w, const int h)", copyToTexture, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxGetTextureSize(const int, int &out w, int &out h)", getTextureSize, gfx)

	// Image loading/saving
	AS_SINGLETON_FUNCTION(X2DRender, "int gfxLoadImage(const string &in, const TextureFilter filter = NearestFilter, const bool mipmap = false)", loadImage, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "array<uint> @gfxLoadImageData(const string &in, int &out, int &out)", loadImageData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxExportTexture(const int, const string &in)", exportTexture, gfx)

	// Texture atlasing
	//AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadImageIntoAtlas(const string &in)", loadImageIntoAtlas, gfx)

	// Shader
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxLoadShader(const string &in, const string &in)", loadShader, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform1i(const string &in, const int)", setUniform1i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform2i(const string &in, const int, const int)", setUniform2i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform3i(const string &in, const int, const int, const int)", setUniform3i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform4i(const string &in, const int, const int, const int, const int)", setUniform4i, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform1f(const string &in, const float)", setUniform1f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform2f(const string &in, const float, const float)", setUniform2f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform3f(const string &in, const float, const float, const float)", setUniform3f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniform4f(const string &in, const float, const float, const float, const float)", setUniform4f, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetUniformTexture(const string &in, const int)", setUniformTexture, gfx)

	// Frame buffers
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateFrameBuffer()", createFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBindFrameBuffer(const int fboId)", bindFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxAttachTextureToFrameBuffer(const int textureId)", attachTextureToFrameBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxClearFrameBuffer()", clearFrameBuffer, gfx)

	// Vertex buffers
	AS_SINGLETON_FUNCTION(X2DRender, "int  gfxCreateVertexBuffer()", createVertexBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxBindVertexBuffer(const int vboId)", bindVertexBuffer, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetVertexBufferData(const array<float> &in, const VertexBufferMode mode)", setVertexBufferData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxSetVertexBufferSubData(const int offset, const array<float> &in)", setVertexBufferSubData, gfx)
	AS_SINGLETON_FUNCTION(X2DRender, "void gfxDrawVertexBuffer(const int mode, const int begin, const int count)", drawVertexBuffer, gfx)*/

	
	// Refresh rate
	//r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "void setRefreshRate(const int)", asMETHOD(xdGraphics, setRefreshRate), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "int  getRefreshRate()", asMETHOD(xdGraphics, getRefreshRate), asCALL_THISCALL); AS_ASSERT
	
	// Vsync
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "void enableVsync()", asMETHOD(xdGraphics, enableVsync), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "void disableVsync()", asMETHOD(xdGraphics, disableVsync), asCALL_THISCALL); AS_ASSERT

	// Time delta
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "float get_dt() const", asMETHOD(xdGraphics, getTimeStep), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "float get_FPS() const", asMETHOD(xdGraphics, getFPS), asCALL_THISCALL); AS_ASSERT

	return 0;
}

xdGraphics::xdGraphics() :
	m_framesPerSecond(0.0f)
{
	setRefreshRate(60);
}

void xdGraphics::setRefreshRate(const int hz)
{
	if(hz == 0)
	{
		ERR("appSetRefreshRate() refresh rate cannot be 0");
		return;
	}
	m_timeStep = 1.0f/hz;
	m_refreshRate = hz;
}

int xdGraphics::getRefreshRate() const
{
	return m_refreshRate;
}

float xdGraphics::getTimeStep() const
{
	return m_timeStep;
}

float xdGraphics::getFPS() const
{
	return m_framesPerSecond;
}

Texture* xdGraphics::CreateTexture(const Pixmap &pixmap)
{
	return s_this->createTexture(pixmap);
}

Texture* xdGraphics::CreateTexture(const string &filePath)
{
	uint width = 0, height = 0;
	uchar *byteData;
	xdAssetLoader::s_this->loadImage(filePath, &byteData, width, height);
	Vector4 *colors = new Vector4[width*height];

	for(int i = 0; i < width*height*4; i += 4) {
		colors[i/4].set( // I dunno why FreeImage loads images as BGRA, but it does.
			byteData[i+2]/255.0f,
			byteData[i+1]/255.0f,
			byteData[i+0]/255.0f,
			byteData[i+3]/255.0f
			);
	}

	Texture *texture = s_this->createTexture(Pixmap(width, height, colors));

	delete[] byteData;
	delete[] colors;

	return texture;
}
	
Texture* xdGraphics::CreateTexture(const int width, const int height)
{
	return s_this->createTexture(Pixmap(width, height));
}
	
Texture* xdGraphics::CreateTexture(const Texture &texture)
{
	return s_this->createTexture(texture.getPixmap());
}

Shader* xdGraphics::CreateShader(const string &vertFilePath, const string &fragFilePath)
{
	return s_this->createShader(vertFilePath, fragFilePath);
}

VertexBufferObject *xdGraphics::CreateVertexBufferObject()
{
	return s_this->createVertexBufferObject();
}

FrameBufferObject *xdGraphics::CreateFrameBufferObject()
{
	return s_this->createFrameBufferObject();
}

bool xdGraphics::IsSupported(Feature feature)
{
	return s_this->isSupported(feature);
}