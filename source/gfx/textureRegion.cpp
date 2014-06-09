//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "textureRegion.h"
#include "texture.h"
#include <x2d/graphics.h>

AS_REG_REF(TextureRegion)

int TextureRegion::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_FACTORY, "TextureRegion @f(Texture @)", asFUNCTIONPR(Factory, (Texture*), TextureRegion*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_FACTORY, "TextureRegion @f(Texture @, const Vector2 &in, const Vector2 &in)", asFUNCTIONPR(Factory, (Texture*, const Vector2&, const Vector2&), TextureRegion*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_FACTORY, "TextureRegion @f(Texture @, const float, const float, const float, const float)", asFUNCTIONPR(Factory, (Texture*, const float, const float, const float, const float), TextureRegion*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv0", offsetof(TextureRegion, uv0)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv1", offsetof(TextureRegion, uv1)); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const Vector2 &in, const Vector2 &in)", asMETHODPR(TextureRegion, setRegion, (const Vector2&, const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const float, const float, const float, const float)", asMETHODPR(TextureRegion, setRegion, (const float, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "Texture @getTexture() const", asMETHODPR(TextureRegion, getTexture, () const, Texture*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setTexture(Texture @)", asMETHODPR(TextureRegion, setTexture, (Texture*), void), asCALL_THISCALL); AS_ASSERT

	return r;
}

TextureRegion::TextureRegion(Texture *texture, const Vector2 &uv0, const Vector2 &uv1) :
	refCounter(this),
	uv0(uv0),
	uv1(uv1),
	texture(texture)
{
}

void TextureRegion::setTexture(Texture *texture)
{
	this->texture = texture;
}

Texture *TextureRegion::getTexture() const
{
	return texture;
}

Vector2i TextureRegion::getSize() const
{
	return Vector2i(int(texture->getWidth()*uv1.x - texture->getWidth()*uv0.x),
		int(texture->getHeight()*uv1.y - texture->getHeight()*uv0.y));
}
	
void TextureRegion::setRegion(const Vector2 &uv0, const Vector2 &uv1)
{
	this->uv0 = uv0;
	this->uv1 = uv1;
}

void TextureRegion::setRegion(const float u0, const float v0, const float u1, const float v1)
{
	uv0.set(u0, v0);
	uv1.set(u1, v1);
}