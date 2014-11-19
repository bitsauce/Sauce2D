#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

class XDAPI XPixmap
{
public:
	XPixmap();
	XPixmap(const uint width, const uint height);
	XPixmap(const uint width, const uint height, const uchar *data);
	XPixmap(const XPixmap& other);
	~XPixmap();

	uint getWidth() const;
	uint getHeight() const;

	XColor getColor(const uint x, const uint y) const;
	void setColor(const uint x, const uint y, const XColor &color);

	void fill(const XColor &color);
	void clear();

	void exportToFile(const string &path) const;

	const uchar *getData() const;

	//Pixmap &operator=(Pixmap &other);

private:
	uchar *m_data;
	uint m_width;
	uint m_height;
};

#endif // X2D_PIXMAP_H