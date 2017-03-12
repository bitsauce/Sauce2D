#ifndef BUTTON_H
#define BUTTON_H

#include "UiObject.h"

class Button : public UiObject
{
public:
	Button(UiObject *parent, const uint btnWidth, const uint btnHeight);

	void onClick(ClickEvent *e);
	void onDraw(DrawEvent *e);

private:
	Resource<Texture2D> m_texture, m_textureHover, m_textureActive;
	Resource<Font> m_font;
	string m_text;
	RenderTarget2D *m_renderTarget;
	SpriteBatch *m_spriteBatch;
};

#endif // BUTTON_H