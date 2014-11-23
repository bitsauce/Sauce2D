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

XTextureRegion::XTextureRegion() :
	uv0(0.0f),
	uv1(1.0f),
	texture(0)
{
}

XTextureRegion::XTextureRegion(const shared_ptr<XTexture> &texture) :
	uv0(0.0f),
	uv1(1.0f),
	texture(texture)
{
}

XTextureRegion::XTextureRegion(const shared_ptr<XTexture> &texture, const Vector2 &uv0, const Vector2 &uv1) :
	uv0(uv0),
	uv1(uv1),
	texture(texture)
{
}

XTextureRegion::XTextureRegion(const shared_ptr<XTexture> &texture, const float u0, const float v0, const float u1, const float v1) :
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
	texture = other.texture;
	
	return *this;
}

XTextureRegion::~XTextureRegion()
{
	//if(texture) {
		//texture->release();
	//}
}

void XTextureRegion::setTexture(const shared_ptr<XTexture> &texture)
{
	//if(this->texture) {
	//	this->texture->release();
	//}
	this->texture = texture;
}

shared_ptr<XTexture> XTextureRegion::getTexture() const
{
	//if(texture) {
	//	texture->addRef();
	//}
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