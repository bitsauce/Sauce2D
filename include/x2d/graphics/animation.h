#ifndef X2D_ANIMATION_H
#define X2D_ANIMATION_H

#include "../engine.h"
#include "textureRegion.h"

BEGIN_XD_NAMESPACE

class XDAPI SpriteAnimation
{
public:
	SpriteAnimation(const int nRows, const int nColumns);
	~SpriteAnimation();

	TextureRegion getKeyFrame(int frameIndex);

private:
	vector<TextureRegion> m_textureRegions;
};

END_XD_NAMESPACE

#endif // X2D_ANIMATION_H