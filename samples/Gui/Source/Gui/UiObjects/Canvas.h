#pragma once

#include "UiObject.h"

class Canvas : public UiObject
{
	friend class Gui;
public:
	Canvas(Window *window, const bool transparent = false);

	void onDraw(DrawEvent *e);
	void onWindowSizeChanged(WindowEvent *e);

	bool isTransparent() const;
	RenderTarget2D *getRenderTarget() const { return m_renderTarget; }

private:
	const bool m_transparent;
	RenderTarget2D *m_renderTarget;
};