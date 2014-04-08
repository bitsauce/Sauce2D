//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "texture.h"
#include "pixmap.h"
#include "color.h"

#include <x2d/graphics.h>

AS_REGISTER_REF(Texture)

int Texture::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	//--------------------------------------------------------------------------------------------
	// Behaviour
	//--------------------------------------------------------------------------------------------
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const string &in)", asFUNCTIONPR(Graphics::CreateTexture, (const string&), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const int, const int)", asFUNCTIONPR(Graphics::CreateTexture, (const int, const int), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Pixmap &in)", asFUNCTIONPR(Graphics::CreateTexture, (const Pixmap&), Texture*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Texture", asBEHAVE_FACTORY, "Texture @f(const Texture @)", asFUNCTIONPR(Graphics::CreateTexture, (const Texture&), Texture*), asCALL_CDECL); AS_ASSERT
	
	//--------------------------------------------------------------------------------------------
	// Enums
	//--------------------------------------------------------------------------------------------
	r = scriptEngine->RegisterEnum("TextureFilter"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "Nearest", xdNearest); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("TextureFilter", "Linear", xdLinear); AS_ASSERT

	//--------------------------------------------------------------------------------------------
	// Methods
	//--------------------------------------------------------------------------------------------
	r = scriptEngine->RegisterObjectMethod("Texture", "void enableMipmaps()", asMETHODPR(Texture, enableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void disableMipmaps()", asMETHODPR(Texture, disableMipmaps, (), void), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Texture", "void setFiltering(const TextureFilter filter)", asMETHODPR(Texture, setFiltering, (const xdTextureFilter), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "Pixmap getPixmap() const", asMETHODPR(Texture, getPixmap, () const, Pixmap), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Texture", "void updatePixmap(const Pixmap &in)", asMETHODPR(Texture, updatePixmap, (const Pixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void updatePixmap(const int, const int, const Pixmap &in)", asMETHODPR(Texture, updatePixmap, (const int, const int, const Pixmap &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Texture", "void clear(c)", asMETHODPR(Texture, clear, (), void), asCALL_THISCALL); AS_ASSERT

	return r;
}

Texture::Texture() :
	refCounter(this)
{
}