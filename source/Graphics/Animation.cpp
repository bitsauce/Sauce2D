//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

SpriteAnimation::SpriteAnimation(const int nRows, const int nColumns)
{
	// Create texture regions
	for(int y = 0; y < nRows; y++)
	{
		for(int x = 0; x < nColumns; x++)
		{
			m_textureRegions.push_back(
				TextureRegion(
					Vector2F(float(x)/nColumns, float(y)/nRows),
					Vector2F(float(x+1)/nColumns, float(y+1)/nRows))
				);
		}
	}
}

SpriteAnimation::~SpriteAnimation()
{
	m_textureRegions.clear();
}

TextureRegion SpriteAnimation::getKeyFrame(uint frameIndex)
{
	return frameIndex < m_textureRegions.size() ? m_textureRegions[frameIndex] : TextureRegion();
}

END_SAUCE_NAMESPACE
