//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "color.h"

AS_REGISTER(Color)

int Color::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	AS_REGISTER_MEMBER(Color, "uint8 r", r);
	AS_REGISTER_MEMBER(Color, "uint8 g", g);
	AS_REGISTER_MEMBER(Color, "uint8 b", b);
	AS_REGISTER_MEMBER(Color, "uint8 a", a);

	return r;
}

Color::Color() :
	r(0), g(0), b(0), a(0)
{
}

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) :
	r(r), g(g), b(b), a(a)
{
}

void Color::set(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}