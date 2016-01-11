#include "LineEdit.h"

// TODO:
// - onResize should be invoked when a parent UiObject is resized
// - Should probably use a better (read: binary) search strategy in getTextIndexAtPosition()
// - Clipboard interaction
// Also, for some reason the positioning system doesn't work anymore. So fix it!

LineEdit::LineEdit(GraphicsContext *gfx, UiObject *parent) :
	UiObject(parent),
	m_cursor(this),
	m_cursorTime(0.0f),
	m_offsetX(0.0f),
	m_font(ResourceManager::get<Font>("Font.fnt")),
	m_textureActive(ResourceManager::get<Texture2D>("Input_Active.png?PremultiplyAlpha")),
	m_textureInactive(ResourceManager::get<Texture2D>("Input_Inactive.png?PremultiplyAlpha")),
	m_renderTarget(0),
	m_dirty(true),
	m_spriteBatch(gfx, 100)
{
}

LineEdit::~LineEdit()
{
}

void LineEdit::setText(const string &text)
{
	m_text = text;
	m_cursor.setPosition(m_text.size());
	m_dirty = true;
}

string LineEdit::getText() const
{
	return m_text;
}

void LineEdit::setTextColor(const Color &color)
{
	m_font->setColor(color);
}

void LineEdit::onTick(TickEvent *e)
{
	m_cursorTime -= e->getDelta();
	if(m_cursorTime <= 0.0f)
	{
		m_cursorTime = 1.0f;
	}
	UiObject::onTick(e);
}

void LineEdit::onDraw(DrawEvent *e)
{
	RectF rect = getDrawRect();
	GraphicsContext *g = e->getGraphicsContext();

	Vector2F textOffset = Vector2F(8.0f - m_offsetX, rect.size.y * 0.5f - m_font->getHeight() * 0.5f);

	if(m_dirty)
	{
		// Update line edit visualization
		g->setRenderTarget(m_renderTarget);

		if(isFocused())
		{
			g->setTexture(m_textureActive);
		}
		else
		{
			g->setTexture(m_textureInactive);
		}

		const float w = m_renderTarget->getWidth(), h = m_renderTarget->getHeight();
		g->drawRectangle(0.0f,      0.0f,      16.0f, 16.0f, Color(255), TextureRegion(0.0f, 0.0f, 1.0f / 3.0f, 1.0f / 3.0f));
		g->drawRectangle(w - 16.0f, 0.0f,      16.0f, 16.0f, Color(255), TextureRegion(2.0f / 3.0f, 0.0f, 1.0f, 1.0f / 3.0f));
		g->drawRectangle(0.0f,      h - 16.0f, 16.0f, 16.0f, Color(255), TextureRegion(0.0f, 2.0f / 3.0f, 1.0f / 3.0f, 1.0f));
		g->drawRectangle(w - 16.0f, h - 16.0f, 16.0f, 16.0f, Color(255), TextureRegion(2.0f / 3.0f, 2.0f / 3.0f, 1.0f, 1.0f));
		g->drawRectangle(16.0f,     0.0f,      w - 32.0f, 16.0f,     Color(255), TextureRegion(1.0f / 3.0f, 0.0f / 3.0f, 2.0f / 3.0f, 1.0f / 3.0f));
		g->drawRectangle(16.0f,     h - 16.0f, w - 32.0f, 16.0f,     Color(255), TextureRegion(1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
		g->drawRectangle(0.0f,      16.0f,     16.0f,     h - 32.0f, Color(255), TextureRegion(0.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f));
		g->drawRectangle(w - 16.0f, 16.0f,     16.0f,     h - 32.0f, Color(255), TextureRegion(2.0f / 3.0f, 1.0f / 3.0f, 3.0f / 3.0f, 2.0f / 3.0f));
		g->drawRectangle(16.0f, 16.0f, w - 32.0f, h - 32.0f, Color(255), TextureRegion(1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));
		g->setTexture(0);

		int begin = getTextIndexAtPosition(rect.position);
		int end = getTextIndexAtPosition(rect.position + rect.size);
		string visibleText = m_text.substr(begin, end - begin);
		float dx = m_font->getStringWidth(m_text.substr(0, end)) - m_font->getStringWidth(visibleText);

		g->enableScissor(8, 0, w - 16, h);
		m_spriteBatch.begin();
		m_font->draw(&m_spriteBatch, textOffset.x + dx, textOffset.y, visibleText);
		m_spriteBatch.end();
		g->disableScissor();

		g->setRenderTarget(0);

		m_dirty = false;
	}

	// Draw line edit visualization
	g->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));
	g->setTexture(m_renderTarget->getTexture());
	g->drawRectangle(rect);
	g->setTexture(0);
	g->setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));

	// Draw text cursor
	if(isFocused() && m_cursorTime >= 0.5f)
	{
		g->drawRectangle(
			rect.position.x + textOffset.x + m_font->getStringWidth(m_text.substr(0, m_cursor.getPosition())),
			rect.position.y + textOffset.y,
			2, m_font->getHeight(),
			m_font->getColor()
			);
	}

	g->drawRectangle(
		rect.position.x + textOffset.x + m_font->getStringWidth(m_text.substr(0, m_cursor.getSelectionStart())),
		rect.position.y + textOffset.y,
		m_font->getStringWidth(m_text.substr(m_cursor.getSelectionStart(), m_cursor.getSelectionLength())),
		m_font->getHeight(),
		Color(0, 0, 0, 127)
		);
}

void LineEdit::onResize(ResizeEvent *e)
{
	delete m_renderTarget;
	m_renderTarget = new RenderTarget2D(e->getWidth(), e->getHeight());
	m_dirty = true;
}

void LineEdit::onFocus(FocusEvent * e)
{
	// Mark as dirty
	m_dirty = true;
}

void LineEdit::insertAt(const int pos, const string &str)
{
	// Check valid index
	if(pos > m_text.size())
		return;

	// Insert string at index
	string endStr = m_text.substr(pos);
	m_text = m_text.substr(0, pos);
	m_text += str + endStr;

	// Mark as dirty
	m_dirty = true;
}

void LineEdit::removeAt(const int pos, const int length)
{
	// Check valid index
	if(pos > m_text.size())
		return;

	// Remove char at index
	string endStr = m_text.substr(pos + length);
	m_text = m_text.substr(0, pos);
	m_text += endStr;

	// Mark as dirty
	m_dirty = true;
}

int LineEdit::getTextIndexAtPosition(Vector2I pos)
{
	RectI rect = getDrawRect();
	pos -= rect.position;
	pos -= Vector2F(8.0f - m_offsetX, rect.size.y * 0.5f - m_font->getHeight() * 0.5f);
	for(int i = 0; i < (int) m_text.size(); ++i)
	{
		if(pos.x < m_font->getStringWidth(m_text, i + 1))
		{
			return i;
		}
	}
	return m_text.size();
}

void LineEdit::updateOffset()
{
	int w = m_renderTarget->getWidth();
	float cursorPos = m_font->getStringWidth(m_text.substr(0, m_cursor.getPosition()));
	if(cursorPos - m_offsetX > w - 16.0f)
	{
		m_offsetX += (cursorPos - m_offsetX) - (w - 16.0f);
		m_dirty = true;
	}
	else if(cursorPos - m_offsetX < 0.0f)
	{
		m_offsetX += cursorPos - m_offsetX;
		m_dirty = true;
	}
}

void LineEdit::onTextInput(TextEvent *e)
{
	// Only add text if active
	if(!isFocused()) return;

	if(m_cursor.getSelectionLength() > 0)
	{
		// Remove selected text
		removeAt(m_cursor.getSelectionStart(), m_cursor.getSelectionLength());
		m_cursor.setPosition(m_cursor.getSelectionStart());
	}

	// Add text
	insertAt(m_cursor.getPosition(), string("") + e->getChar());
	m_cursor.moveCursor(1);
}

void LineEdit::onKeyEvent(KeyEvent *e)
{
	// Only add text if active
	if(!isFocused() || e->getType() == KeyEvent::UP) return;

	const int cursorPos = m_cursor.getPosition();
	const int selectLen = m_cursor.getSelectionLength();
	const int selectPos = m_cursor.getSelectionStart();
	const bool useAnchor = (e->getModifiers() & KeyEvent::SHIFT) != 0;

	switch(e->getKeycode())
	{
		// Enter
		case CGF_KEY_RETURN:
		{
			// Call accept function
			if(m_acceptFunc)
			{
				m_acceptFunc();
			}
			break;
		}

		// Backspace
		case CGF_KEY_BACKSPACE:
		{
			if(selectLen == 0)
			{
				// Remove char behind
				if(cursorPos != 0)
				{
					removeAt(cursorPos - 1);
					m_cursor.moveCursor(-1);
				}
			}
			else
			{
				removeAt(selectPos, selectLen);
				m_cursor.setPosition(selectPos);
			}
			break;
		}

		// Delete
		case CGF_KEY_DELETE:
		{
			if(selectLen == 0)
			{
				// Remove char in front
				if(cursorPos < (int) m_text.size())
				{
					removeAt(cursorPos);
				}
			}
			else
			{
				removeAt(selectPos, selectLen);
				m_cursor.setPosition(selectPos);
			}
		}
		break;

		// Left cursor key
		case CGF_KEY_LEFT:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// Move cursor to the left of the previous word
				int tmp = cursorPos;
				while(tmp > 0 && m_text[--tmp] == ' ');
				while(tmp > 0 && m_text[tmp - 1] != ' ') tmp--;
				m_cursor.setPosition(tmp, useAnchor);
			}
			else
			{
				// Move cursor one step to the left
				m_cursor.moveCursor(-1, useAnchor);
			}
		}
		break;

		// Right cursor key
		case CGF_KEY_RIGHT:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// Move cursor to the left of the next word
				int tmp = cursorPos;
				while((uint) tmp < m_text.size() && m_text[++tmp] != ' ');
				while((uint) tmp < m_text.size() && m_text[tmp] == ' ') tmp++;
				m_cursor.setPosition(tmp, useAnchor);
			}
			else
			{
				// Move cursor one step to the right
				m_cursor.moveCursor(1, useAnchor);
			}
		}
		break;

		// Home key
		case CGF_KEY_HOME:
		{
			m_cursor.setPosition(0, useAnchor);
		}
		break;

		// End key
		case CGF_KEY_END:
		{
			m_cursor.setPosition(m_text.size(), useAnchor);
		}
		break;

		// Copy
		case CGF_KEY_C:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// TODO
			}
		}

		// Cut
		case CGF_KEY_X:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// TODO
			}
		}

		// Paste
		case CGF_KEY_V:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				//string str = Input::getClipboardString();
				//insertAt(m_cursorPos, str);
				//m_cursorPos += str.size();
			}
		}
		break;
	}

	m_cursorTime = 1.0f;
}

void LineEdit::onClick(ClickEvent *e)
{
	m_cursor.setPosition(getTextIndexAtPosition(e->getMouseEvent()->getPosition()));
	m_cursorTime = 1.0f;
}
