#pragma once

#include "Gui/Gui.h"

class SinglePlayerCanvas : public Canvas
{
public:
	SinglePlayerCanvas(Gui *gui, Window *window);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	LineEdit *m_lineEdit;
	Button *m_buttonBack;
	Button *m_buttonOk;
	Slider *m_slider;
};