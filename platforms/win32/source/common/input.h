#ifndef WIN_INPUT_H
#define WIN_INPUT_H

#include <x2d/x2d.h>

class Input : public XInput
{
	friend class Window;
public:

	Input();

	// Cursor functions
	void setCursorPos(const Vector2i &pos);
	Vector2i getCursorPos() const;
	void setCursorLimits(const Recti &area);

	// Position
	Vector2 getPosition() const;

	// Keyboard
	bool getKeyState(const XVirtualKey key) const;

private:
	Vector2 m_position;
};

extern XVirtualKey fromWinKey(uchar key);
extern uchar toWinKey(XVirtualKey key);

#endif // WIN_INPUT_H