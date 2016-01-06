#ifndef CANVAS_H
#define CANVAS_H

#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(Window *window);
	Canvas(Window *window, const int width, const int height);

	Vector2 getSize() const;

private:
	Window *m_window;
	const int m_canvasWidth, m_canvasHeight;
	const bool m_useWindowSize;
};

#endif // CANVAS_H