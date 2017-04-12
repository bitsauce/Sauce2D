#pragma once

#include "Config.h"
#include "UiEvents.h"

#include "UiObjects/AspectRatioContainer.h"
#include "UiObjects/Button.h"
#include "UiObjects/LineEdit.h"
#include "UiObjects/Canvas.h"
#include "UiObjects/Background.h"
#include "UiObjects/DialogBox.h"
#include "UiObjects/Slider.h"
#include "UiObjects/Label.h"

class Canvas;

class Gui : public SceneObject
{
public:
	Gui();

	void onEvent(Event *e);
	void onDraw(DrawEvent *e);
	void onTick(TickEvent *e);

	void pushCanvas(Canvas *canvas);
	bool popCanvas();

private:
	list<Canvas*> m_canvas;
	float m_transitionTime;
	int m_transitionDirection;
	const float m_fadeTime;
};