//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

XGraphics *XGraphics::s_this = 0;

uint QUAD_INDICES[6] = {
	0, 3, 1, 1, 3, 2
};

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

vector<XRenderContext**> XGraphics::s_contextToCreate;
XVertexFormat XVertexFormat::s_vct;
mutex ctxmtx;

AS_REG_SINGLETON(XGraphics)

int XGraphics::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	// Vsync
	r = scriptEngine->RegisterObjectMethod("XGraphics", "void enableVsync()", asMETHOD(XGraphics, enableVsync), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XGraphics", "void disableVsync()", asMETHOD(XGraphics, disableVsync), asCALL_THISCALL); AS_ASSERT

	// Time delta
	r = scriptEngine->RegisterObjectMethod("XGraphics", "float get_dt() const", asMETHOD(XGraphics, getTimeStep), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XGraphics", "float get_FPS() const", asMETHOD(XGraphics, getFPS), asCALL_THISCALL); AS_ASSERT

	return 0;
}

XGraphics::XGraphics() :
	m_framesPerSecond(0.0f)
{
	setRefreshRate(60);

	XVertexFormat::s_vct.set(VERTEX_POSITION, 2);
	XVertexFormat::s_vct.set(VERTEX_COLOR, 4, XD_UBYTE);
	XVertexFormat::s_vct.set(VERTEX_TEX_COORD, 2);
}

void XGraphics::setRefreshRate(const int hz)
{
	if(hz == 0)
	{
		LOG("appSetRefreshRate() refresh rate cannot be 0");
		return;
	}
	m_timeStep = 1.0f/hz;
	m_refreshRate = hz;
}

int XGraphics::getRefreshRate() const
{
	return m_refreshRate;
}

float XGraphics::getTimeStep() const
{
	return m_timeStep;
}

float XGraphics::getFPS() const
{
	return m_framesPerSecond;
}

void XGraphics::CreateContext(XRenderContext **context)
{
	ctxmtx.lock();
	s_contextToCreate.push_back(context);
	ctxmtx.unlock();
}

XRenderContext* XGraphics::CreateContext()
{
	return s_this->createContext();
}

void XGraphics::DestroyContext(XRenderContext *context)
{
	return s_this->destroyContext(context);
}

XTexture* XGraphics::CreateTexture(const XPixmap &pixmap)
{
	return s_this->createTexture(pixmap);
}

XTexture* XGraphics::CreateTexture(const string &filePath)
{
	XTexture *texture = 0;
	XPixmap *pixmap = XAssetManager::LoadPixmap(filePath);

	if(pixmap)
	{
		texture = s_this->createTexture(*pixmap);
	}
	else
	{
		LOG("XGraphics::CreateTexture() - Unable to load image file '%s'", filePath.c_str());
	}

	delete pixmap;
	return texture;
}
	
XTexture* XGraphics::CreateTexture(const int width, const int height)
{
	return s_this->createTexture(XPixmap(width, height));
}
	
XTexture* XGraphics::CreateTexture(const XTexture &texture)
{
	return s_this->createTexture(texture.getPixmap());
}

XShader* XGraphics::CreateShader(const string &vertFilePath, const string &fragFilePath)
{
	return s_this->createShader(vertFilePath, fragFilePath);
}

XVertexBufferObject *XGraphics::CreateVertexBufferObject(XVertexBuffer *buffer)
{
	return s_this->createVertexBufferObject(*buffer);
}

XFrameBufferObject *XGraphics::CreateFrameBufferObject()
{
	return s_this->createFrameBufferObject();
}

bool XGraphics::IsSupported(Feature feature)
{
	return s_this->isSupported(feature);
}