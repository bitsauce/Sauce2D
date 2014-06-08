#ifndef WIN_INPUT_H
#define WIN_INPUT_H

#include <x2d/input.h>

class Input : public xdInput
{
public:

	// Cursor functions
	void cursorPos(int &x, int &y) const;
	void setCursorPos(const int x, const int y);
	void setCursorLimits(const int x, const int y, const int w, const int h);

	// Keyboard
	bool getKeyState(const xdVirtualKey key);
};

#endif // WIN_INPUT_H