//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/graphics/texture.h>
#include <x2d/graphics/pixmap.h>

#include <x2d/graphics.h>

AS_REG_REF(Texture)

int Texture::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const string &in)", asFUNCTIONPR(xdGraphics::CreateTexture, (const string&), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const int, const int)", asFUNCTIONPR(xdGraphics::CreateTexture, (const int, const int), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Pixmap &in)", asFUNCTIONPR(xdGraphics::CreateTexture, (const Pixmap&), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Texture @)", asFUNCTIONPR(xdGraphics::CreateTexture, (const Texture&), Texture*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterEnum("TextureFilter"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "NEAREST", xdNearest); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "LINEAR", xdLinear); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Texture", "void enableMipmaps()", asMETHODPR(Texture, enableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void disableMipmaps()", asMETHODPR(Texture, disableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void setFiltering(const TextureFilter filter)", asMETHODPR(Texture, setFiltering, (const xdTextureFilter), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "Pixmap getPixmap() const", asMETHODPR(Texture, getPixmap, () const, Pixmap), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void update(const Pixmap &in)", asMETHODPR(Texture, updatePixmap, (const Pixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void updateSection(const int, const int, const Pixmap &in)", asMETHODPR(Texture, updatePixmap, (const int, const int, const Pixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void clear()", asMETHODPR(Texture, clear, (), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Texture", "int getWidth() const", asMETHOD(Texture, getWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "int getHeight() const", asMETHOD(Texture, getHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "Vector2i getSize() const", asMETHOD(Texture, getSize), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

Texture::Texture()
{
}