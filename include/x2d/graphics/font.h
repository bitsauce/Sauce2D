#ifndef X2D_FONT_H
#define X2D_FONT_H

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

class SpriteBatch;

class Font;
typedef shared_ptr<Font> FontPtr;

class FontLoader;
class Texture2D;

struct XDAPI CharDescr
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

class XDAPI Font
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

template XDAPI class shared_ptr<Font>;

END_XD_NAMESPACE

#endif // X2D_FONT_H