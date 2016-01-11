#include "LineEdit.h"

// TODO:
// - onResize should be invoked when a parent UiObject is resized
// - Should probably use a better (read: binary) search strategy in getTextIndexAtPosition()
// - Clipboard interaction
// - Text selection
// Also, for some reason the positioning system doesn't work anymore. So fix it!

LineEdit::LineEdit(GraphicsContext *gfx, UiObject *parent) :
	UiObject(parent),
	m_cursorPos(0),
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
	setCursorPos(m_text.size());
	m_dirty = true;
}

string LineEdit::getText() const
{
	return m_text;
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
			rect.position.x + textOffset.x + m_font->getStringWidth(m_text.substr(0, m_cursorPos)),
			rect.position.y + textOffset.y,
			2, m_font->getHeight(),
			m_font->getColor()
			);
	}
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

void LineEdit::insertAt(const uint at, const string &str)
{
	// Check valid index
	if(at > m_text.size())
		return;

	// Insert string at index
	string endStr = m_text.substr(at);
	m_text = m_text.substr(0, at);
	m_text += str + endStr;

	// Mark as dirty
	m_dirty = true;
}

void LineEdit::removeAt(const uint at)
{
	// Check valid index
	if(at > m_text.size())
		return;

	// Remove char at index
	string endStr = m_text.substr(at);
	m_text = m_text.substr(0, at - 1);
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

void LineEdit::moveCursor(const int dt)
{
	setCursorPos(m_cursorPos + dt);
}

void LineEdit::setCursorPos(const int pos)
{
	m_cursorPos = pos;

	int w = m_renderTarget->getWidth();
	float cursorPos = m_font->getStringWidth(m_text.substr(0, m_cursorPos));
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

	// Add text
	insertAt(m_cursorPos, string("") + e->getChar());
	moveCursor(1);
}

void LineEdit::onKeyEvent(KeyEvent *e)
{
	// Only add text if active
	if(!isFocused() || e->getType() == KeyEvent::UP) return;

	int prevCursorPos = m_cursorPos;

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
			// Remove char behind
			if(m_cursorPos != 0)
			{
				removeAt(m_cursorPos);
				m_cursorPos--;
			}
			break;
		}

		// Delete
		case CGF_KEY_DELETE:
		{
			// Remove char in front
			if(m_cursorPos + 1 <= (int) m_text.size())
			{
				removeAt(m_cursorPos + 1);
			}
		}
		break;

		// Left cursor key
		case CGF_KEY_LEFT:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// Move cursor to the left of the previous word
				while(m_cursorPos > 0 && m_text[--m_cursorPos] == ' ');
				while(m_cursorPos > 0 && m_text[m_cursorPos - 1] != ' ') m_cursorPos--;
			}
			else
			{
				// Move cursor one step to the left
				m_cursorPos = max(m_cursorPos - 1, 0);
			}
		}
		break;

		// Right cursor key
		case CGF_KEY_RIGHT:
		{
			if((e->getModifiers() & KeyEvent::CTRL) != 0)
			{
				// Move cursor to the left of the next word
				while((uint) m_cursorPos < m_text.size() && m_text[++m_cursorPos] != ' ');
				while((uint) m_cursorPos < m_text.size() && m_text[m_cursorPos] == ' ') m_cursorPos++;
			}
			else
			{
				// Move cursor one step to the right
				m_cursorPos = min(m_cursorPos + 1, (int) m_text.size());
			}
		}
		break;

		// Home key
		case CGF_KEY_HOME:
		{
			m_cursorPos = 0;
		}
		break;

		// End key
		case CGF_KEY_END:
		{
			m_cursorPos = m_text.size();
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

	if(prevCursorPos != m_cursorPos)
	{
		setCursorPos(m_cursorPos);
	}

	m_cursorTime = 1.0f;
}

void LineEdit::onClick(ClickEvent *e)
{
	m_cursorPos = getTextIndexAtPosition(e->getMouseEvent()->getPosition());
	m_cursorTime = 1.0f;
}
