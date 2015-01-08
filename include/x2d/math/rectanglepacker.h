#ifndef X2D_RECTANGLE_PACKER_H
#define X2D_RECTANGLE_PACKER_H

#include "../config.h"

#include "vector.h"
#include "rect.h"

BEGIN_XD_NAMESPACE

class XDAPI RectanglePacker
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

	class XDAPI Rectangle
	{
	public:
		Rectangle(void *data) :
			data(data),
			x(0),
			y(0),
			width(0),
			height(0)
		{
		}

		void set(int x, int y, int width, int height)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}

		void setSize(int width, int height)
		{
			this->width = width;
			this->height = height;
		}

		int x, y, width, height;
		const void *getData() const { return data; }

	private:
		void *data;
	};

	struct XDAPI Result
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
		vector<const Rectangle> rectangles;
	};

	const Result pack();
	void addRect(const Rectangle rectangle);
	void clearRects();

private:
	vector<Rectangle> m_rectangles;
	int m_maxWidth;
};

END_XD_NAMESPACE

#endif // X2D_RECTANGLE_PACKER_H