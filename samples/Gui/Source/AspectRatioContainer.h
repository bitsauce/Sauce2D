#ifndef CANVAS_H
#define CANVAS_H

#include "UiObject.h"

class AspectRatioContainer : public UiObject
{
public:
	AspectRatioContainer(UiObject *parent, Window *window, const int maxWidth, const float aspectRatio);

	void onWindowSizeChanged(WindowEvent *e);
	void onDraw(DrawEvent *e);

private:
	Window *m_window;
	const int m_maxWidth;
	const float m_aspectRatio;
};

#endif // CANVAS_H