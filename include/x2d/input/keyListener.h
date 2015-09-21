#ifndef X2D_KEY_LISTENER_H
#define X2D_KEY_LISTENER_H

#include <x2d/config.h>

BEGIN_XD_NAMESPACE

class XDAPI KeyListener
{
	friend class Window;
public:
	KeyListener();
	virtual ~KeyListener();

	virtual void keyPressEvent(const VirtualKey key) { }
	virtual void keyReleaseEvent(const VirtualKey key) { }
	virtual void charEvent(const uint c) { }

private:

	static void callCharEvent(const uint c);
	static void callKeyPressed(const VirtualKey key);
	static void callKeyReleased(const VirtualKey key);

	static list<KeyListener*> s_keyListeners;
};

END_XD_NAMESPACE

#endif // X2D_KEY_LISTENER_H