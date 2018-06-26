//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

TextureRegion::TextureRegion() :
	uv0(0.0f),
	uv1(1.0f)
{
}
TextureRegion::TextureRegion(const Vector2F &uv0, const Vector2F &uv1) :
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
	
void TextureRegion::setRegion(const Vector2F &uv0, const Vector2F &uv1)
{
	this->uv0 = uv0;
	this->uv1 = uv1;
}

void TextureRegion::setRegion(const float u0, const float v0, const float u1, const float v1)
{
	uv0.set(u0, v0);
	uv1.set(u1, v1);
}

END_SAUCE_NAMESPACE
