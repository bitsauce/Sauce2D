#ifndef GFX_PIXMAP_H
#define GFX_PIXMAP_H

#include <x2d/base.h>

struct Color;

class Pixmap
{
public:
	AS_VALUE_CLASS(Pixmap)
	AS_CONSTRUCTOR_ARG2(Pixmap, const int, const int)
	AS_CONSTRUCTOR_ARG3(Pixmap, const int, const int, const Color*)
	AS_CONSTRUCTOR_ARG1(Pixmap, const Pixmap&)

	Pixmap(const int width, const int height);
	Pixmap(const int width, const int height, const Color *pixels);
	~Pixmap();

	int getWidth() const;
	int getHeight() const;

	Color getColor(const int x, const int y) const;
	void setColor(const int x, const int y, const Color &color);

	void fill(const Color &color);
	void clear();

	const char *getData() const;

private:
	Color *m_data;
	int m_width;
	int m_height;
};

#endif // GFX_PIXMAP_H