#ifndef CGF_VIEWPORT_H
#define CGF_VIEWPORT_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class CGF_API Viewport
{
public:
	Viewport(const Rect<Sint32> &view);
	Viewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Rect<Sint32> m_viewport;
};

END_CGF_NAMESPACE

#endif // CGF_VIEWPORT_H
