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

AS_REG_POD(XTextureRegion, "TextureRegion")

int XTextureRegion::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(Construct, (XTextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture@)", asFUNCTIONPR(Construct, (XTexture*, XTextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture@, const Vector2 &in, const Vector2 &in)", asFUNCTIONPR(Construct, (XTexture*, const Vector2&, const Vector2&, XTextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(Texture@, const float, const float, const float, const float)", asFUNCTIONPR(Construct, (XTexture*, const float, const float, const float, const float, XTextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_CONSTRUCT, "void f(const TextureRegion &in)", asFUNCTIONPR(Construct, (const XTextureRegion&, XTextureRegion*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("TextureRegion", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Destruct), asCALL_CDECL_OBJLAST); AS_ASSERT
	
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv0", offsetof(XTextureRegion, uv0)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("TextureRegion", "Vector2 uv1", offsetof(XTextureRegion, uv1)); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const Vector2 &in, const Vector2 &in)", asMETHODPR(XTextureRegion, setRegion, (const Vector2&, const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setRegion(const float, const float, const float, const float)", asMETHODPR(XTextureRegion, setRegion, (const float, const float, const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "Texture @getTexture() const", asMETHODPR(XTextureRegion, getTexture, () const, XTexture*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "void setTexture(Texture@)", asMETHODPR(XTextureRegion, setTexture, (XTexture*), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("TextureRegion", "TextureRegion &opAssign(const TextureRegion &in)", asMETHOD(XTextureRegion, operator=), asCALL_THISCALL); AS_ASSERT

	return r;
}

XTextureRegion::XTextureRegion() :
	uv0(0.0f),
	uv1(1.0f),
	texture(0)
{
}

XTextureRegion::XTextureRegion(XTexture *texture, const Vector2 &uv0, const Vector2 &uv1) :
	uv0(uv0),
	uv1(uv1),
	texture(texture)
{
}

XTextureRegion::XTextureRegion(XTexture *texture, const float u0, const float v0, const float u1, const float v1) :
	uv0(u0, v0),
	uv1(u1, v1),
	texture(texture)
{
}

XTextureRegion::XTextureRegion(const XTextureRegion &other) :
	uv0(0.0f),
	uv1(0.0f),
	texture(0)
{
	*this = other;
}

XTextureRegion &XTextureRegion::operator=(const XTextureRegion &other)
{
	if(this == &other) {
		return *this;
	}
	
	uv0 = other.uv0;
	uv1 = other.uv1;
	if(texture != other.texture)
	{
		if(texture) {
			texture->release();
		}
		texture = other.texture;
		if(texture) {
			texture->addRef();
		}
	}
	
	return *this;
}

XTextureRegion::~XTextureRegion()
{
	if(texture) {
		texture->release();
	}
}

void XTextureRegion::setTexture(XTexture *texture)
{
	if(this->texture) {
		this->texture->release();
	}
	this->texture = texture;
}

XTexture *XTextureRegion::getTexture() const
{
	if(texture) {
		texture->addRef();
	}
	return texture;
}

Vector2i XTextureRegion::getSize() const
{
	return texture != 0 ? Vector2i(
		int(texture->getWidth()*uv1.x - texture->getWidth()*uv0.x),
		int(texture->getHeight()*uv1.y - texture->getHeight()*uv0.y)
		) : Vector2i(0);
}
	
void XTextureRegion::setRegion(const Vector2 &uv0, const Vector2 &uv1)
{
	this->uv0 = uv0;
	this->uv1 = uv1;
}

void XTextureRegion::setRegion(const float u0, const float v0, const float u1, const float v1)
{
	uv0.set(u0, v0);
	uv1.set(u1, v1);
}