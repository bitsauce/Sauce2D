#ifndef X2D_ANIMATION_H
#define X2D_ANIMATION_H

#include <x2d/engine.h>
#include <x2d/graphics/textureregion.h>

BEGIN_CG_NAMESPACE

class XDAPI SpriteAnimation
{
public:
	SpriteAnimation(const int nRows, const int nColumns);
	~SpriteAnimation();

	TextureRegion getKeyFrame(uint frameIndex);

private:
	vector<TextureRegion> m_textureRegions;
};

END_CG_NAMESPACE

#endif // X2D_ANIMATION_H