#ifndef X2D_VIEWPORT_H
#define X2D_VIEWPORT_H

#include "../engine.h"

class XViewport
{
public:
	XViewport(const Recti &view);
	XViewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Recti m_viewport;
};

#endif // X2D_VIEWPORT_H