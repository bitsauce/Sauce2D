#ifndef X2D_MOUSE_LISTENER_H
#define X2D_MOUSE_LISTENER_H

#include <x2d/config.h>

BEGIN_XD_NAMESPACE

class XDAPI MouseListener
{
	friend class Window;
public:
	MouseListener();
	virtual ~MouseListener();

	virtual void mouseWheelEvent(const int delta) { }

private:
	static void callMouseWheelEvent(const int delta);

	static list<MouseListener*> s_mouseListeners;
};

END_XD_NAMESPACE

#endif // X2D_MOUSE_LISTENER_H