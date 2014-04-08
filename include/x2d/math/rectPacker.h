#ifndef GFX_TEXTURE_PACKER_H
#define GFX_TEXTURE_PACKER_H

#include "x2d/platform.h"
#include "x2d/math.h"

class X2DAPI RectPacker
{
public:
	class X2DAPI Rect : public Recti
	{
	public:
		Rect(void *data) :
			data(data)
		{
		}

		void *getData()
		{
			return data;
		}

	private:
		void *data;
	};

	struct X2DAPI Result
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
			rectList.clear();
		}

		bool valid;
		Vector2i canvas;
		int area;
		float efficiency;
		vector<Rect> rectList;
	};

	Result pack();
	void addRect(const Rect rect);
	void clearRects();

private:
	vector<Rect> rectList;
};

#endif // GFX_TEXTURE_PACKER_H