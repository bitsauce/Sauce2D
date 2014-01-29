//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		MixedGraphics (C)

#include "x2d/math/rectPacker.h"
#include "x2d/gfx.h"

bool heightSort(RectPacker::Rect i, RectPacker::Rect j)
{
	return i.getHeight() > j.getHeight();
}

RectPacker::Result RectPacker::pack()
{
	// No point in packing 0 rectangles
	if(rectList.size() == 0)
		return Result();

	// Sort rectangles by height
	sort(rectList.begin(), rectList.end(), heightSort);

	// Find total area and max height
	int totalArea = 0;
	int rightMost = 0;
	int maxWidth = 0, maxHeight = rectList[0].getHeight();
	for(uint i = 0; i < rectList.size(); i++)
	{
		totalArea += rectList[i].getArea();
		if(maxWidth < rectList[i].getWidth())
			maxWidth = rectList[i].getWidth();
	}

	// Setup loop vars
	int canvasWidth = 8192, canvasHeight = maxHeight;
	vector<rect2i> cells;
	cells.push_back(rect2i(0, 0, canvasWidth, canvasHeight));
	Result bestResult;
	Result result;

	// Find optimal canvas rect by testing
	int idx = 0;
	while(canvasWidth >= maxWidth)
	{
		// Get rectangle
		Rect rect = rectList[idx++];

		// Find best cell
		int bestCellIdx = -1;
		for(uint i = 0; i < cells.size(); i++)
		{
			rect2i *cell = &cells[i];
			if(cell->getWidth() >= rect.getWidth() && cell->getHeight() >= rect.getHeight())
			{
				if(bestCellIdx < 0 || cell->getArea() < cells[bestCellIdx].getArea())
					bestCellIdx = i;
			}
		}

		if(bestCellIdx < 0)
		{
			// If no cell was found, add more to the height and retry
			cells.clear();
			cells.push_back(rect2i(0, 0, canvasWidth, ++canvasHeight));
			result.clear();
			idx = rightMost = 0;
		}else{
			// Store cell and remove
			rect2i cell = cells[bestCellIdx];
			cells.erase(cells.begin() + bestCellIdx);
			
			// Place rectangle into results
			rect.set(cell.getX(), cell.getY(), rect.getWidth(), rect.getHeight());
			result.rectList.push_back(rect);
			if(rect.getRight() > rightMost)
				rightMost = rect.getRight();
				
			// Split the cell into 2 smaller cells
			rect2i c1(cell.getX() + rect.getWidth(), cell.getY(), cell.getWidth() - rect.getWidth(), rect.getHeight());   // +1 for padding pixel
			rect2i c2(cell.getX(), cell.getY() + rect.getHeight(), cell.getWidth(), cell.getHeight() - rect.getHeight()); // +1 for padding pixel
				
			if(c1.getArea() > 0) cells.push_back(c1);
			if(c2.getArea() > 0) cells.push_back(c2);
				
			// Check for end
			if(idx == (int)rectList.size())
			{
				// Success, check for the smalest area
				result.area = rightMost*canvasHeight;
				if(!bestResult.valid || result.area < bestResult.area)
				{
					// Set start width
					if(!bestResult.valid)
						canvasWidth = rightMost;
						
					// Store canvas dimentions and the result as best
					result.valid = true;
					result.canvas.set(rightMost, canvasHeight);
					bestResult = result;
				}
					
				// Retry with different dimentions
				cells.clear();
				cells.push_back(rect2i(0, 0, --canvasWidth, canvasHeight));
				result.clear();
				idx = rightMost = 0;
			}
		}
	}
	return bestResult;
}

void RectPacker::addRect(const Rect rect)
{
	rectList.push_back(rect);
}

void RectPacker::clearRects()
{
	rectList.clear();
}