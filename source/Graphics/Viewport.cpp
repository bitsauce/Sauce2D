//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

Viewport::Viewport(const Rect<int> &view)
{
}

Viewport::Viewport(const int x, const int y, const int w, const int h)
{
}

void Viewport::makeCurrent() const
{
	//Gfx::setViewport(this);
}

END_SAUCE_NAMESPACE