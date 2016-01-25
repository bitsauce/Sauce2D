#ifndef BUTTON_H
#define BUTTON_H

#include "UiObject.h"

class Button : public UiObject
{
public:
	Button(UiObject *parent);

	void onClick(ClickEvent *e);
	void onDraw(DrawEvent *e);

private:
	Texture2DPtr m_texture, m_textureHover, m_textureActive;
	FontPtr m_font;
	string m_text;
};

#endif // BUTTON_H