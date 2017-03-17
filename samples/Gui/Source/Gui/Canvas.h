#pragma once

#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(Window *window, const bool transparent = false);

	void onDraw(DrawEvent *e);
	void onWindowSizeChanged(WindowEvent *e);

	bool isTransparent() const;

private:
	const bool m_transparent;
};