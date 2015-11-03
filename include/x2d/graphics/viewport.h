#ifndef X2D_VIEWPORT_H
#define X2D_VIEWPORT_H

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

class XDAPI Viewport
{
public:
	Viewport(const Recti &view);
	Viewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Recti m_viewport;
};

END_XD_NAMESPACE

#endif // X2D_VIEWPORT_H