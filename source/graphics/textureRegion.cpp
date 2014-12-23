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

namespace xd
{

TextureRegion::TextureRegion() :
	uv0(0.0f),
	uv1(1.0f)
{
}
TextureRegion::TextureRegion(const Vector2 &uv0, const Vector2 &uv1) :
	uv0(uv0),
	uv1(uv1)
{
}

TextureRegion::TextureRegion(const float u0, const float v0, const float u1, const float v1) :
	uv0(u0, v0),
	uv1(u1, v1)
{
}

TextureRegion::TextureRegion(const TextureRegion &other) :
	uv0(0.0f),
	uv1(0.0f)
{
	*this = other;
}

TextureRegion &TextureRegion::operator=(const TextureRegion &other)
{
	if(this == &other) {
		return *this;
	}
	
	uv0 = other.uv0;
	uv1 = other.uv1;
	
	return *this;
}

TextureRegion::~TextureRegion()
{
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

}