#pragma once

#include "UiObject.h"

class Slider : public UiObject
{
public:
	Slider(UiObject *parent);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	float m_percentage;
};