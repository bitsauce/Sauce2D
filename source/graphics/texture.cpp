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

AS_REG_REF(XTexture, "Texture")

int XTexture::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const string &in)", asFUNCTIONPR(XGraphics::CreateTexture, (const string&), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const int, const int)", asFUNCTIONPR(XGraphics::CreateTexture, (const int, const int), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Pixmap &in)", asFUNCTIONPR(XGraphics::CreateTexture, (const XPixmap&), XTexture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Texture @)", asFUNCTIONPR(XGraphics::CreateTexture, (const XTexture&), XTexture*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterEnum("TextureFilter"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "NEAREST", xdNearest); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "LINEAR", xdLinear); AS_ASSERT

	r = scriptEngine->RegisterEnum("TextureWrapping"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureWrapping", "CLAMP_TO_BORDER", CLAMP_TO_BORDER); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureWrapping", "CLAMP_TO_EDGE", CLAMP_TO_EDGE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureWrapping", "REPEAT", REPEAT); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureWrapping", "MIRRORED_REPEAT", MIRRORED_REPEAT); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Texture", "void enableMipmaps()", asMETHODPR(XTexture, enableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void disableMipmaps()", asMETHODPR(XTexture, disableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void setFiltering(const TextureFilter)", asMETHODPR(XTexture, setFiltering, (const XTextureFilter), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void setWrapping(const TextureWrapping)", asMETHOD(XTexture, setWrapping), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "Pixmap getPixmap() const", asMETHODPR(XTexture, getPixmap, () const, XPixmap), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void update(const Pixmap &in)", asMETHODPR(XTexture, updatePixmap, (const XPixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void updateSection(const int, const int, const Pixmap &in)", asMETHODPR(XTexture, updatePixmap, (const int, const int, const XPixmap&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void clear()", asMETHODPR(XTexture, clear, (), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Texture", "int getWidth() const", asMETHOD(XTexture, getWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "int getHeight() const", asMETHOD(XTexture, getHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "Vector2i getSize() const", asMETHOD(XTexture, getSize), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

XTexture::XTexture()
{
}