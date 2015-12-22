#ifndef CGF_ANIMATION_H
#define CGF_ANIMATION_H

#include <CGF/Common.h>
#include <cgf/graphics/textureregion.h>

BEGIN_CGF_NAMESPACE

class CGF_API SpriteAnimation
{
public:
	SpriteAnimation(const int nRows, const int nColumns);
	~SpriteAnimation();

	TextureRegion getKeyFrame(uint frameIndex);

private:
	vector<TextureRegion> m_textureRegions;
};

END_CGF_NAMESPACE

#endif // CGF_ANIMATION_H
