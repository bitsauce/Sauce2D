#ifndef GFX_FONT_H
#define GFX_FONT_H

#include <x2d/platform.h>
#include <x2d/base.h>

class Font
{
public:
	AS_REF_CLASS(Font)
	AS_FACTORY_ARG1(Font, const string&)

	Font(const string &filePathOrFontName);

	float getStringWidth(const string &str);
	float getStringHeight(const string &str);

	void draw(const string &str);

private:
};

#endif // GFX_FONT_H