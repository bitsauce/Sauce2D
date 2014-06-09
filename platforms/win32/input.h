#ifndef WIN_INPUT_H
#define WIN_INPUT_H

#include <x2d/input.h>

class Input : public xdInput
{
public:

	// Cursor functions
	void setCursorPos(const Vector2i &pos);
	Vector2i getCursorPos() const;
	void setCursorLimits(const Recti &area);

	// Keyboard
	bool getKeyState(const xdVirtualKey key) const;
};

#endif // WIN_INPUT_H