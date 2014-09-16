//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "common/engine.h"
#include "pixmap.h"
#include "texture.h"

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

AS_REG_SINGLETON(XGraphics)

int XGraphics::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	// Vsync
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "void enableVsync()", asMETHOD(XGraphics, enableVsync), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "void disableVsync()", asMETHOD(XGraphics, disableVsync), asCALL_THISCALL); AS_ASSERT

	// Time delta
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "float get_dt() const", asMETHOD(XGraphics, getTimeStep), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptGraphics", "float get_FPS() const", asMETHOD(XGraphics, getFPS), asCALL_THISCALL); AS_ASSERT

	return 0;
}

XGraphics::XGraphics() :
	m_framesPerSecond(0.0f)
{
	setRefreshRate(60);
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

XTexture* XGraphics::CreateTexture(const XPixmap &pixmap)
{
	return s_this->createTexture(pixmap);
}

XTexture* XGraphics::CreateTexture(const string &filePath)
{
	uint width = 0, height = 0;
	uchar *byteData;
	XTexture *texture = 0;

	int r = 0;
	// TODO: REFACTORING
	if(/*(r = XAssetManager::LoadPixmap(filePath, &byteData, width, height)) >= 0*/false)
	{
		Vector4 *colors = new Vector4[width*height];

		for(uint i = 0; i < width*height*4; i += 4) {
			colors[i/4].set( // I dunno why FreeImage loads images as BGRA, but it does.
				byteData[i+2]/255.0f,
				byteData[i+1]/255.0f,
				byteData[i+0]/255.0f,
				byteData[i+3]/255.0f
				);
		}

		texture = s_this->createTexture(XPixmap(width, height, colors));

		delete[] byteData;
		delete[] colors;
	}else
	{
		LOG("XGraphics::CreateTexture() - Unable to load image file '%s' (error code %i)", filePath.c_str(), r);
	}
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

XVertexBufferObject *XGraphics::CreateVertexBufferObject()
{
	return s_this->createVertexBufferObject();
}

XFrameBufferObject *XGraphics::CreateFrameBufferObject()
{
	return s_this->createFrameBufferObject();
}

bool XGraphics::IsSupported(Feature feature)
{
	return s_this->isSupported(feature);
}