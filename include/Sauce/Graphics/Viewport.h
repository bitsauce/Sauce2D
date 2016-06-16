#ifndef SAUCE_VIEWPORT_H
#define SAUCE_VIEWPORT_H

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API Viewport
{
public:
	Viewport(const Rect<Sint32> &view);
	Viewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Rect<Sint32> m_viewport;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_VIEWPORT_H
