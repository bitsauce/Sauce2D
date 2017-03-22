#pragma once

#include "Gui/Gui.h"
#include "Gui/Canvas.h"
#include "Gui/LineEdit.h"
#include "Gui/AspectRatioContainer.h"
#include "Gui/Background.h"
#include "Gui/Button.h"
#include "Gui/DialogBoxCanvas.h"

class SinglePlayerCanvas : public Canvas
{
public:
	SinglePlayerCanvas(Gui *gui, Window *window);

private:
	AspectRatioContainer *m_aspectRatioContainer;
	LineEdit *m_lineEdit;
	Button *m_buttonBack;
	Button *m_buttonOk;
};