//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		MixedGraphics (C)

#include "gfx/atlas.h"
#include "x2d/gfx.h"

int nextPow2(int n)
{
	int x = 1;
	while(x < n) x <<= 1;
	return x;
}  

TextureAtlas::TextureAtlas()
{
	// Get gfx
	/*X2DRender *gfx = gameEngine->getGfx();

	// Calculate the largest possible textures on the current hardware
	int maxPixels;
	gfx->getMaxTextureSize(); //glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxPixels);
	maxPixels /= 4; // Assuming we're using 32-bits textures

	// Store the length one side of the texture
	m_sides = nextPow2(int(sqrt((float)maxPixels)))/2;
	
	// Create texture
	gfx->createTexture(); //glGenTextures(1, &m_textureId);

	// Set texture data
	vector<uint> textureData(0, m_sides*m_sides);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_sides, m_sides, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, (void*)textureData.data());
	glBindTexture(GL_TEXTURE_2D, 0);*/
}