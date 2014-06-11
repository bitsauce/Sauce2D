#ifndef GFX_VIEWPORT_H
#define GFX_VIEWPORT_H

#include <x2d/math.h>
#include <x2d/base.h>

class Viewport
{
public:
	AS_DECL_REF
	AS_FACTORY_ARG1(Viewport, const Recti&)
	AS_FACTORY_ARG4(Viewport, const int, const int, const int, const int)

	Viewport(const Recti &view);
	Viewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Recti m_viewport;
};

#endif // GFX_VIEWPORT_H