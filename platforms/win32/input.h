#ifndef WIN_INPUT_H
#define WIN_INPUT_H

#include <x2d/input.h>

class Input : public xdInput
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
	bool getKeyState(const xdVirtualKey key) const;

private:
	Vector2 m_position;
};

extern xdVirtualKey fromWinKey(uchar key);
extern uchar toWinKey(xdVirtualKey key);

#endif // WIN_INPUT_H