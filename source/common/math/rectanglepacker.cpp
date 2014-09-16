//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
// Inspired by: http://www.codeproject.com/Articles/210979/Fast-optimizing-rectangle-packing-algorithm-for-bu (i think)

#include "rectanglepacker.h"

bool heightSort(RectanglePacker::Rectangle i, RectanglePacker::Rectangle j)
{
	return i.height > j.height;
}

const RectanglePacker::Result RectanglePacker::pack()
{
	// No point in packing 0 rectangles
	if(m_rectangles.size() == 0)
		return Result();

	// Sort rectangles by height
	sort(m_rectangles.begin(), m_rectangles.end(), heightSort);

	// Find total area and max height
	int totalArea = 0;
	int rightMost = 0;
	int maxWidth = 0, maxHeight = m_rectangles[0].height;
	for(uint i = 0; i < m_rectangles.size(); i++)
	{
		Rectangle &rect = m_rectangles[i];
		totalArea += rect.height * rect.height;
		if(maxWidth < rect.width) {
			maxWidth = rect.width;
		}
	}

	// Setup loop vars
	int canvasWidth = 8192, canvasHeight = maxHeight;
	vector<Recti> cells;
	cells.push_back(Recti(0, 0, canvasWidth, canvasHeight));
	Result bestResult;
	Result result;

	// Find optimal canvas rect by testing
	int idx = 0;
	while(canvasWidth >= maxWidth)
	{
		// Get rectangle
		Rectangle &rect = m_rectangles[idx++];

		// Find best cell
		int bestCellIdx = -1;
		for(uint i = 0; i < cells.size(); i++)
		{
			Recti *cell = &cells[i];
			if(cell->getWidth() >= rect.width && cell->getHeight() >= rect.height)
			{
				if(bestCellIdx < 0 || cell->getArea() < cells[bestCellIdx].getArea())
					bestCellIdx = i;
			}
		}

		if(bestCellIdx < 0)
		{
			// If no cell was found, add more to the height and retry
			cells.clear();
			cells.push_back(Recti(0, 0, canvasWidth, ++canvasHeight));
			result.clear();
			idx = rightMost = 0;
		}else{
			// Store cell and remove
			Recti cell = cells[bestCellIdx];
			cells.erase(cells.begin() + bestCellIdx);
			
			// Place rectangle into results
			rect.set(cell.getX(), cell.getY(), rect.width, rect.height);
			result.rectangles.push_back(rect);
			if(rect.x + rect.width > rightMost) {
				rightMost = rect.x + rect.width;
			}
				
			// Split the cell into 2 smaller cells
			Recti c1(cell.getX() + rect.width, cell.getY(), cell.getWidth() - rect.width, rect.height);   // +1 for padding pixel
			Recti c2(cell.getX(), cell.getY() + rect.height, cell.getWidth(), cell.getHeight() - rect.height); // +1 for padding pixel
				
			if(c1.getArea() > 0) cells.push_back(c1);
			if(c2.getArea() > 0) cells.push_back(c2);
				
			// Check for end
			if(idx == (int)m_rectangles.size())
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
				cells.push_back(Recti(0, 0, --canvasWidth, canvasHeight));
				result.clear();
				idx = rightMost = 0;
			}
		}
	}
	return bestResult;
}

void RectanglePacker::addRect(const Rectangle rect)
{
	m_rectangles.push_back(rect);
}

void RectanglePacker::clearRects()
{
	m_rectangles.clear();
}