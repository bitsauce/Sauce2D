#ifndef X2D_ANIMATION_H
#define X2D_ANIMATION_H

#include "../engine.h"

class XTexture;
class XTextureRegion;

class XDAPI XAnimation
{
public:
	XAnimation(XTexture *texture, const int nRows, const int nColumns);
	~XAnimation();

	XTextureRegion getKeyFrame(int frameIndex);

private:
	vector<XTextureRegion> m_textureRegions;

	static XAnimation *Factory(XTexture *texture, const int nRows, const int nColumns)
	{
		return new XAnimation(texture, nRows, nColumns);
	}
};

#endif // X2D_ANIMATION_H