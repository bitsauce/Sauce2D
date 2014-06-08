#ifndef GFX_FONT_H
#define GFX_FONT_H

#include <x2d/config.h>
#include <x2d/base.h>
#include <x2d/math.h>
#include "batch.h"
#include "texture.h"

class Font
{
public:
	AS_DECL_REF

	Font(const string &fontNameOrFile);

	float getStringWidth(const string &str);
	float getStringHeight(const string &str);
	void setColor(const Vector4 &color);
	void draw(Batch &batch, const Vector2 &pos, const string &str);

	struct Char
	{
		Char() :
			advance(0.0f),
			pos(0.0f),
			size(0.0f),
			texCoord0(0.0f),
			texCoord1(0.0f)
		{
		}

		float advance;
		Vector2 pos;
		Vector2 size;
		Vector2 texCoord0;
		Vector2 texCoord1;
	};

private:
	AS_FACTORY_ARG1(Font, const string&)

	Vector4 m_color;
	Texture *m_texture;
	int m_size;
	vector<Char> m_chars;
};

#endif // GFX_FONT_H