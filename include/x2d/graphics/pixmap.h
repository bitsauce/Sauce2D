#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

class XDAPI XPixmap
{
public:
	AS_DECL_POD

	XPixmap();
	XPixmap(const int width, const int height);
	XPixmap(const int width, const int height, const Vector4 *pixels);
	XPixmap(const int width, const int height, const XScriptArray &pixels);
	XPixmap(const XPixmap& other);
	~XPixmap();

	int getWidth() const;
	int getHeight() const;

	Vector4 getColor(const int x, const int y) const;
	void setColor(const int x, const int y, const Vector4 &color);

	void fill(const Vector4 &color);
	void clear();

	void exportToFile(const string &path) const;

	const float *getData() const;

	//Pixmap &operator=(Pixmap &other);

private:
	Vector4 *m_data;
	int m_width;
	int m_height;
	
	static void Factory(XPixmap &other, XPixmap *self) { new (self) XPixmap(other); }
	static void Factory(const int width, const int height, XPixmap *self) { new (self) XPixmap(width, height); }
	static void Factory(const int width, const int height, XScriptArray &arr, XPixmap *self) {
		new (self) XPixmap(width, height, arr);
	}
};

#endif // X2D_PIXMAP_H