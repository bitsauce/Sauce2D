#include "animation.h"
#include <x2d/graphics/textureRegion.h>
#include <x2d/graphics/texture.h>

AS_REG_REF(Animation)

int Animation::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Animation", asBEHAVE_FACTORY, "Animation @f(Texture @, const int, const int)", asFUNCTIONPR(Factory, (Texture*, const int, const int), Animation*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Animation", "TextureRegion getKeyFrame(int frameIndex) const", asMETHOD(Animation, getKeyFrame), asCALL_THISCALL); AS_ASSERT

	return r;
}

Animation::Animation(Texture *texture, const int nRows, const int nColumns)
{
	// Make sure we're feed a texture
	if(texture == 0)
		return;

	// Create texture regions
	for(int y = 0; y < nRows; y++)
	{
		for(int x = 0; x < nColumns; x++)
		{
			texture->addRef();
			m_textureRegions.push_back(
				TextureRegion(texture,
					Vector2(float(x)/nColumns, float(y)/nRows),
					Vector2(float(x+1)/nColumns, float(y+1)/nRows))
				);
		}
	}

	// Release argument ref
	texture->release();
}

Animation::~Animation()
{
	m_textureRegions.clear();
}

TextureRegion Animation::getKeyFrame(int frameIndex)
{
	return m_textureRegions[frameIndex];
}