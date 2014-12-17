#ifndef X2D_ANIMATION_H
#define X2D_ANIMATION_H

#include "../engine.h"
#include "texture.h"

namespace xd
{

class TextureRegion;

class XDAPI Animation
{
public:
	Animation(Texture2DPtr &texture, const int nRows, const int nColumns);
	~Animation();

	TextureRegion getKeyFrame(int frameIndex);

private:
	vector<TextureRegion> m_textureRegions;
};

}

#endif // X2D_ANIMATION_H