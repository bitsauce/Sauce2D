//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "viewport.h"

AS_REG_REF(Viewport)

int Viewport::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	AS_REGISTER_FACTORY(Viewport, "const Recti &in", (const Recti &))
	AS_REGISTER_FACTORY(Viewport, "const int, const int, const int, const int", (const int, const int, const int, const int))

	AS_REGISTER_METHOD(Viewport, "void makeCurrent() const", makeCurrent, () const, void)

	return r;
}

Viewport::Viewport(const Recti &view) :
	refCounter(this)
{
}

Viewport::Viewport(const int x, const int y, const int w, const int h) :
	refCounter(this)
{
}

void Viewport::makeCurrent() const
{
	//Gfx::setViewport(this);
}