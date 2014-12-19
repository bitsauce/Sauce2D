#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

namespace xd
{

// Pixel formats
enum PixelFormat
{
	ALPHA = GL_ALPHA,
	LUMINANCE = GL_LUMINANCE,
	LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
	RGB = GL_RGB,
	RGBA = GL_RGBA
};

extern size_t getPixelFormatSize(const PixelFormat format);

class XDAPI Pixmap
{
public:
	Pixmap(const PixelFormat format = RGBA);
	Pixmap(const uint width, const uint height, const PixelFormat format = RGBA);
	Pixmap(const uint width, const uint height, const uchar *data, const PixelFormat format = RGBA);
	Pixmap(const Pixmap& other);
	~Pixmap();

	uint getWidth() const;
	uint getHeight() const;
	PixelFormat getFormat() const;

	Color getColor(const uint x, const uint y) const;
	void setColor(const uint x, const uint y, const Color &color);

	void fill(const Color &color);
	void clear();

	void exportToFile(const string &path) const;

	const uchar *getData() const;

	//Pixmap &operator=(Pixmap &other);

private:
	uchar *m_data;
	uint m_width;
	uint m_height;
	PixelFormat m_format;
	uint m_pixelSize;
};

}

#endif // X2D_PIXMAP_H