#ifndef CGF_RECTANGLE_PACKER_H
#define CGF_RECTANGLE_PACKER_H

#include <CGF/Config.h>

#include <CGF/Math/Vector.h>
#include <CGF/Math/Rectangle.h>

BEGIN_CGF_NAMESPACE

class CGF_API RectanglePacker
{
	friend class TextureAtlas;
public:
	RectanglePacker() :
		m_maxWidth(2048)
	{
	}

	void setMaxWidth(const int width)
	{
		m_maxWidth = width;
	}

	class CGF_API Rect
	{
		friend class RectanglePacker;
	public:
		Rect(const uint width, const uint height, void *data) :
			data(data),
			x(0),
			y(0),
			width(width),
			height(height)
		{
		}

		Rect(const Rect &other) :
			data(other.data),
			x(other.x),
			y(other.y),
			width(other.width),
			height(other.height)
		{
		}

		uint getX() const
		{
			return x;
		}

		uint getY() const
		{
			return y;
		}

		uint getWidth() const
		{
			return width;
		}

		uint getHeight() const
		{
			return height;
		}

		const void *getData() const
		{
			return data;
		}

	private:
		uint x, y;
		uint width, height; // Should be const but cannot be, as it breaks operator=.
		void *data;
	};

	struct CGF_API Result
	{
		Result() :
			valid(false),
			canvas(0),
			area(0),
			efficiency(0.0f)
		{
		}

		void clear()
		{
			canvas.set(0, 0);
			area = 0;
			efficiency = 0.0f;
			rectangles.clear();
		}

		bool valid;
		Vector2i canvas;
		int area;
		float efficiency;
		vector<Rect> rectangles;
	};

	const Result pack();
	void addRect(const Rect rectangle);
	void clearRects();

private:
	vector<Rect> m_rectangles;
	int m_maxWidth;
};

END_CGF_NAMESPACE

#endif // CGF_RECTANGLE_PACKER_H
