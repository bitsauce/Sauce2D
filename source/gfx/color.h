#ifndef GFX_COLOR_H
#define GFX_COLOR_H

//#pragma pack(4)

#include <x2d/base.h>
#include <x2d/math.h>

struct Color
{
	AS_VALUE_CLASS(Color)

	Color();
	Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
	//Color(const string& hexstr);

	void set(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 0xFF);

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

//#pragma pack(pop)

#endif // GFX_COLOR_H