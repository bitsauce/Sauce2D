#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include "UiObject.h"

class UiObject;

class LineEdit : public UiObject
{
public:
	LineEdit(GraphicsContext *gfx, UiObject *parent);
	~LineEdit();

	void setAcceptFunc(function<void()> func)
	{
		m_acceptFunc = func;
	}

	void setText(const string &text);
	string getText() const;

	void setColor(const Color &color)
	{
		m_font->setColor(color);
	}

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onResize(ResizeEvent *e);
	void onFocus(FocusEvent *e);

protected:
	void insertAt(const uint at, const string &str);
	void removeAt(const uint at);
	int getTextIndexAtPosition(Vector2I pos);
	void moveCursor(const int dt);
	void setCursorPos(const int pos);
	void onTextInput(TextEvent *e);
	void onKeyEvent(KeyEvent *e);
	void onClick(ClickEvent *e);

	SpriteBatch m_spriteBatch;
	Texture2DPtr m_textureActive, m_textureInactive;
	RenderTarget2D *m_renderTarget;
	FontPtr m_font;
	Color m_color;
	string m_text;
	int m_cursorPos;
	float m_cursorTime;
	float m_offsetX;
	bool m_dirty;
	function<void()> m_acceptFunc;
};

#endif // LINE_EDIT_H