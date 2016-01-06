#ifndef BOX_H
#define BOX_H

#include "UiObject.h"

class Box : public UiObject
{
public:
	Box();

	void onClick(ClickEvent *e);
	void onDraw(DrawEvent *e);

public:
	Texture2DPtr m_texture, m_textureHover, m_textureActive;
	FontPtr m_font;
	string m_text;
};

#endif // BOX_H