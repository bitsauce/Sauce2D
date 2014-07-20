#ifndef GRAPHICS_ANIMATION_H
#define GRAPHICS_ANIMATION_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/math.h>

class Texture;
class TextureRegion;

class Animation
{
public:
	AS_DECL_REF

	Animation(Texture *texture, const int nRows, const int nColumns);
	~Animation();

	TextureRegion getKeyFrame(int frameIndex);

private:
	vector<TextureRegion> m_textureRegions;

	static Animation *Factory(Texture *texture, const int nRows, const int nColumns) {
		return new Animation(texture, nRows, nColumns);
	}
};

#endif // GRAPHICS_ANIMATION_H