#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

namespace xd
{

class XDAPI Pixmap
{
public:
	Pixmap();
	Pixmap(const uint width, const uint height);
	Pixmap(const uint width, const uint height, const uchar *data);
	Pixmap(const Pixmap& other);
	~Pixmap();

	uint getWidth() const;
	uint getHeight() const;

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
};

}

#endif // X2D_PIXMAP_H