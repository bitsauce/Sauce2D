#ifndef GFX_PIXMAP_H
#define GFX_PIXMAP_H

#include <x2d/math.h>
#include <x2d/base.h>
#include <x2d/scripts/array.h>

class XDAPI Pixmap
{
public:
	AS_DECL_VALUE

	Pixmap(const int width, const int height);
	Pixmap(const int width, const int height, const Vector4 *pixels);
	Pixmap(const int width, const int height, const Array &pixels);
	~Pixmap();

	int getWidth() const;
	int getHeight() const;

	Vector4 getColor(const int x, const int y) const;
	void setColor(const int x, const int y, const Vector4 &color);

	void fill(const Vector4 &color);
	void clear();

	const float *getData() const;

private:
	Vector4 *m_data;
	int m_width;
	int m_height;
	
	static void Factory(Pixmap &other, Pixmap *self) { new (self) Pixmap(other); }
	static void Factory(const int width, const int height, Pixmap *self) { new (self) Pixmap(width, height); }
	static void Factory(const int width, const int height, Array &arr, Pixmap *self) {
		new (self) Pixmap(width, height, arr);
	}
};

#endif // GFX_PIXMAP_H