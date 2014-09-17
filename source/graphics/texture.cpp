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

AS_REG_REF(XTexture)

int XTexture::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("XTexture", asBEHAVE_FACTORY, "XTexture @f(const string &in)", asFUNCTIONPR(XGraphics::CreateTexture, (const string&), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("XTexture", asBEHAVE_FACTORY, "XTexture @f(const int, const int)", asFUNCTIONPR(XGraphics::CreateTexture, (const int, const int), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("XTexture", asBEHAVE_FACTORY, "XTexture @f(const XPixmap &in)", asFUNCTIONPR(XGraphics::CreateTexture, (const XPixmap&), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("XTexture", asBEHAVE_FACTORY, "XTexture @f(const XTexture @)", asFUNCTIONPR(XGraphics::CreateTexture, (const XTexture&), XTexture*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterEnum("XTextureFilter"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureFilter", "NEAREST", xdNearest); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureFilter", "LINEAR", xdLinear); AS_ASSERT

	r = scriptEngine->RegisterEnum("XTextureWrapping"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureWrapping", "CLAMP_TO_BORDER", CLAMP_TO_BORDER); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureWrapping", "CLAMP_TO_EDGE", CLAMP_TO_EDGE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureWrapping", "REPEAT", REPEAT); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("XTextureWrapping", "MIRRORED_REPEAT", MIRRORED_REPEAT); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XTexture", "void enableMipmaps()", asMETHODPR(XTexture, enableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void disableMipmaps()", asMETHODPR(XTexture, disableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void setFiltering(const XTextureFilter)", asMETHODPR(XTexture, setFiltering, (const XTextureFilter), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void setWrapping(const XTextureWrapping)", asMETHOD(XTexture, setWrapping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "XPixmap getPixmap() const", asMETHODPR(XTexture, getPixmap, () const, XPixmap), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void update(const XPixmap &in)", asMETHODPR(XTexture, updatePixmap, (const XPixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void updateSection(const int, const int, const XPixmap &in)", asMETHODPR(XTexture, updatePixmap, (const int, const int, const XPixmap&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "void clear()", asMETHODPR(XTexture, clear, (), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XTexture", "int getWidth() const", asMETHOD(XTexture, getWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "int getHeight() const", asMETHOD(XTexture, getHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XTexture", "Vector2i getSize() const", asMETHOD(XTexture, getSize), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

XTexture::XTexture()
{
}