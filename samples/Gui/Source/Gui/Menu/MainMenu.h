#pragma once

#include "Gui/Gui.h"

class MainMenuCanvas : public Canvas
{
public:
	MainMenuCanvas(Gui *gui, Window *window);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	LineEdit *m_lineEdit;
	Button *m_buttonBack;
	Button *m_buttonOk;
	Slider *m_slider;
};