#pragma once

#include "Canvas.h"
#include "Gui.h"
#include "AspectRatioContainer.h"
#include "Button.h"

class DialogBoxCanvas : public Canvas
{
public:
	DialogBoxCanvas(Gui *gui, Window *window, Canvas*c);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	Button *m_buttonOk;
	Button *m_buttonOk2;
};