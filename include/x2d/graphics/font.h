#ifndef X2D_FONT_H
#define X2D_FONT_H

#include "../engine.h"

namespace xd
{

class Font;
struct Color;
class TextureAtlas;

typedef shared_ptr<Font> FontPtr;

class XDAPI Font
{
public:
	~Font();

	float getStringWidth(const string &str);
	float getStringHeight(const string &str);
	void setColor(const Color &color);
	void draw(Batch *batch, const Vector2 &pos, const string &str);
	Texture2DPtr renderToTexture(const string &text, const uint padding = 2);

	struct CharMetrics
	{
		CharMetrics() :
			advance(0),
			bearing(0),
			size(0)
		{
		}

		// Metrics
		Vector2i advance;
		Vector2i bearing;
		Vector2i size;
	};

	static FontPtr loadResource(const string &name);

private:
	// Load font using TrueType 2
	Font(const string &fontFile, const uint size);
	bool isValidChar(uchar ch) { return ch >= 0 && ch < 128; }

	// Font color
	Color m_color;

	// Font texture atlas
	TextureAtlas *m_atlas;

	// Font size (px)
	int m_size;

	// Line size (px)
	int m_lineSize;

	// Origin
	int m_Msize;

	// Font character metrics
	vector<CharMetrics> m_metrics;
};

template XDAPI class shared_ptr<Font>;

}

#endif // X2D_FONT_H