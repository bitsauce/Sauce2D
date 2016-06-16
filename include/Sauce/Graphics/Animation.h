#ifndef SAUCE_ANIMATION_H
#define SAUCE_ANIMATION_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API SpriteAnimation
{
public:
	SpriteAnimation(const int nRows, const int nColumns);
	~SpriteAnimation();

	TextureRegion getKeyFrame(uint frameIndex);

private:
	vector<TextureRegion> m_textureRegions;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_ANIMATION_H
