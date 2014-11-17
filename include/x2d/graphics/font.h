#ifndef X2D_FONT_H
#define X2D_FONT_H

#include "../engine.h"

class XTextureAtlas;

class XDAPI XFont
{
public:
	XFont(string &fontName, const uint size);
	~XFont();

	float getStringWidth(const string &str);
	float getStringHeight(const string &str);
	void setColor(const Vector4 &color);
	void draw(XBatch *batch, const Vector2 &pos, const string &str);
	XTexture *XFont::renderToTexture(const string &text, const uint padding = 2);

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

private:
	// Load font using TrueType 2
	void load(const string &fontFile, const uint size);
	bool isValidChar(uchar ch) { return ch >= 0 && ch < 128; }

	// Font color
	Vector4 m_color;

	// Font texture atlas
	XTextureAtlas *m_atlas;

	// Font size (px)
	int m_size;

	// Line size (px)
	int m_lineSize;

	// Origin
	int m_Msize;

	// Font character metrics
	vector<CharMetrics> m_metrics;
};

#endif // X2D_FONT_H