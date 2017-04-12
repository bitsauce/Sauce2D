#pragma once

#include "Gui/Gui.h"

class DialogBoxCanvas : public Canvas
{
public:
	DialogBoxCanvas(Gui *gui, Window *window, const string &message/*, Type type = DIALOG_BOX_OK*/);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	Button *m_buttonOk;
	Label *m_message;
};