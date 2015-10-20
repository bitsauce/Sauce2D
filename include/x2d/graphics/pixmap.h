#ifndef X2D_PIXMAP_H
#define X2D_PIXMAP_H

#include "../engine.h"

BEGIN_XD_NAMESPACE

class XDAPI PixelFormat
{
public:
	enum Components
	{
		R = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA
	};

	enum DataType
	{
		INT = GL_INT,
		UNSIGNED_INT = GL_UNSIGNED_INT,
		BYTE = GL_BYTE,
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		FLOAT = GL_FLOAT
	};

	PixelFormat(Components components = RGBA, DataType dataType = UNSIGNED_BYTE)
	{
		m_components = components;
		m_dataType = dataType;
	}

	Components getComponents() const { return m_components; }
	DataType getDataType() const { return m_dataType; }

	uint getComponentCount() const;
	uint getDataTypeSizeInBytes() const;
	uint getPixelSizeInBytes() const;

private:
	Components m_components;
	DataType m_dataType;
};

class XDAPI Pixmap
{
public:

public:
	Pixmap(const PixelFormat &format = PixelFormat());
	Pixmap(const uint width, const uint height, const PixelFormat &format = PixelFormat());
	Pixmap(const uint width, const uint height, const void *data, const PixelFormat &format = PixelFormat());
	Pixmap(const Pixmap& other);
	~Pixmap();

	uint getWidth() const;
	uint getHeight() const;
	PixelFormat getFormat() const;

	void getPixel(const uint x, const uint y, void *data) const;
	void setPixel(const uint x, const uint y, const void *data);

	void fill(const void *data);
	void clear();

	void exportToFile(string path) const;

	const uchar *getData() const;

	//Pixmap &operator=(Pixmap &other);

private:
	uchar *m_data;
	uint m_width;
	uint m_height;
	PixelFormat m_format;
};

END_XD_NAMESPACE

#endif // X2D_PIXMAP_H