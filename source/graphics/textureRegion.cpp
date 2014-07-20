//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/graphics/textureRegion.h>
#include <x2d/graphics/texture.h>
#include <x2d/graphics.h>

AS_REG_VALUE(TextureRegion)

int TextureRegion::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture@)", asFUNCTIONPR(Factory, (Texture*, TextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture @, const Vector2 &in, const Vector2 &in)", asFUNCTIONPR(Factory, (Texture*, const Vector2&, const Vector2&, TextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture @, const float, const float, const float, const float)", asFUNCTIONPR(Factory, (Texture*, const float, const float, const float, const float, TextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv0", offsetof(TextureRegion, uv0)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv1", offsetof(TextureRegion, uv1)); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const Vector2 &in, const Vector2 &in)", asMETHODPR(TextureRegion, setRegion, (const Vector2&, const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const float, const float, const float, const float)", asMETHODPR(TextureRegion, setRegion, (const float, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "Texture @getTexture() const", asMETHODPR(TextureRegion, getTexture, () const, Texture*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setTexture(Texture @)", asMETHODPR(TextureRegion, setTexture, (Texture*), void), asCALL_THISCALL); AS_ASSERT

	return r;
}

TextureRegion::TextureRegion(Texture *texture, const Vector2 &uv0, const Vector2 &uv1) :
	uv0(uv0),
	uv1(uv1),
	texture(texture)
{
}

TextureRegion::TextureRegion(Texture *texture, const float u0, const float v0, const float u1, const float v1) :
	uv0(u0, v0),
	uv1(u1, v1),
	texture(texture)
{
}

TextureRegion::TextureRegion(const TextureRegion &other) :
	uv0(other.uv0),
	uv1(other.uv1),
	texture(other.texture)
{
	if(texture) {
		texture->addRef();
	}
}

/*TextureRegion &TextureRegion::operator=(TextureRegion &other)
{
	swap(uv0, other.uv0);
	swap(uv1, other.uv1);
	swap(texture, other.texture);
	return *this;
}*/

TextureRegion::~TextureRegion()
{
	if(texture) {
		texture->release();
	}
}

void TextureRegion::setTexture(Texture *texture)
{
	if(this->texture) {
		this->texture->release();
	}
	this->texture = texture;
}

Texture *TextureRegion::getTexture() const
{
	if(texture) {
		texture->addRef();
	}
	return texture;
}

Vector2i TextureRegion::getSize() const
{
	return texture != 0 ? Vector2i(
		int(texture->getWidth()*uv1.x - texture->getWidth()*uv0.x),
		int(texture->getHeight()*uv1.y - texture->getHeight()*uv0.y)
		) : Vector2i(0);
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