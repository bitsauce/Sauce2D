#ifndef CGF_FONT_H
#define CGF_FONT_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class SpriteBatch;

class Font;
typedef shared_ptr<Font> FontPtr;

class FontLoader;
class Texture2D;

struct CGF_API CharDescr
{
	CharDescr() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0), page(0) {}

	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;

	std::vector<int> kerningPairs;
};

enum FontTextEncoding
{
	NONE,
	UTF8,
	UTF16
};


enum FontAlign
{
	FONT_ALIGN_LEFT,
	FONT_ALIGN_CENTER,
	FONT_ALIGN_RIGHT,
	FONT_ALIGN_JUSTIFY
};

class CGF_API Font
{
public:
	Font(string fontFile);
	~Font();

	void setTextEncoding(FontTextEncoding encoding);
	void setDepth(const float depth) { m_depth = depth;  }
	void setColor(const Color &color) { m_color = color; }
	Color getColor() { return m_color; }

	float getStringWidth(const string &text, int count = 0);
	float getStringHeight(const string &text);
	void draw(SpriteBatch *spriteBatch, float x, float y, const string &text, FontAlign mode = FONT_ALIGN_LEFT);
	void draw(SpriteBatch *spriteBatch, const Vector2 &pos, const string &text, FontAlign mode = FONT_ALIGN_LEFT) { draw(spriteBatch, pos.x, pos.y, text, mode); }

	/**
	 * \fn	void Font::drawBox(SpriteBatch *spriteBatch, float x, float y, float width, const string &text, int count, FontAlign mode = FONT_ALIGN_LEFT);
	 *
	 * \brief	Draw text to fit within a box. The text will be split when a line exceedes \p width.
	 *
	 * \param [in,out]	spriteBatch	The sprite batch to draw the text to.
	 * \param	x				   	The x coordinate.
	 * \param	y				   	The y coordinate.
	 * \param	width			   	The max lengt of a line.
	 * \param	text			   	The text to draw.
	 * \param	count			   	Number of characters of \p text to draw.
	 * \param	mode			   	The font alignment mode.
	 * \todo Setting \p mode to anything other than FONT_ALIGN_LEFT does nothing at the moment.
	 */

	void drawBox(SpriteBatch *spriteBatch, float x, float y, float width, const string &text, int count, FontAlign mode = FONT_ALIGN_LEFT);
	void drawBox(SpriteBatch *spriteBatch, const Vector2 &pos, float width, const string &text, int count, FontAlign mode = FONT_ALIGN_LEFT) { drawBox(spriteBatch, pos.x, pos.y, width, text, count, mode); }

	void setHeight(float h);
	float getHeight() const;

	float getBottomOffset() const;
	float getTopOffset() const;

	static FontPtr loadResource(const string &fontFile);

protected:
	friend class FontLoader;

	void drawInternal(SpriteBatch *spriteBatch, float x, float y, const string &text, int count, float spacing = 0);

	float adjustForKerningPairs(int first, int second);
	CharDescr *getChar(int id);

	int getTextLength(const string &text);
	int getTextChar(const string &text, int pos, int *nextPos = 0);
	int findTextChar(const char *text, int start, int length, int ch);

	short m_fontHeight; // total height of the font
	short m_base;       // y of base line
	short m_scaleW;
	short m_scaleH;
	CharDescr m_defChar;
	bool m_hasOutline;

	float m_scale;
	float m_depth;
	Color m_color;
	FontTextEncoding m_encoding;

	map<int, CharDescr*> m_chars;
	vector<shared_ptr<Texture2D>> m_pages;
};

template CGF_API class shared_ptr<Font>;

END_CGF_NAMESPACE

#endif // CGF_FONT_H
