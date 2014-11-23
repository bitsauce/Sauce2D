#ifndef X2D_ANIMATION_H
#define X2D_ANIMATION_H

#include "../engine.h"

class XTexture;
class XTextureRegion;

class XDAPI XAnimation
{
public:
	XAnimation(shared_ptr<XTexture> &texture, const int nRows, const int nColumns);
	~XAnimation();

	XTextureRegion getKeyFrame(int frameIndex);

private:
	vector<XTextureRegion> m_textureRegions;
};

#endif // X2D_ANIMATION_H