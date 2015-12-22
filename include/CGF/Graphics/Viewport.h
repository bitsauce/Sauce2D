#ifndef CGF_VIEWPORT_H
#define CGF_VIEWPORT_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class CGF_API Viewport
{
public:
	Viewport(const Recti &view);
	Viewport(const int x, const int y, const int w, const int h);

	void makeCurrent() const;

private:
	Recti m_viewport;
};

END_CGF_NAMESPACE

#endif // CGF_VIEWPORT_H
