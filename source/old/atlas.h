#ifndef GFX_ATLAS_H
#define GFX_ATLAS_H

#include "x2d/platform.h"

class TextureAtlas
{
public:
	TextureAtlas();

	struct SubTexture
	{
		TextureAtlas *owner;
	};

private:
	int m_sides;
	uint m_textureId;
};

#endif // GFX_ATLAS_H