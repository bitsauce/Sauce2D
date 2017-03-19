#pragma once

#include "Config.h"
#include "UiEvents.h"

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