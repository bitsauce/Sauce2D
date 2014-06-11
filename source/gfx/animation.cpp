#include "animation.h"
#include "textureRegion.h"
#include "texture.h"

AS_REG_REF(Animation)

int Animation::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Animation", asBEHAVE_FACTORY, "Animation @f(Texture @, const int, const int)", asFUNCTIONPR(Factory, (Texture*, const int, const int), Animation*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Animation", "TextureRegion @getKeyFrame(int frameIndex) const", asMETHOD(Animation, getKeyFrame), asCALL_THISCALL); AS_ASSERT

	return r;
}

Animation::Animation(Texture *texture, const int nRows, const int nColumns)
{
	for(int y = 0; y < nRows; y++) {
		for(int x = 0; x < nColumns; x++) {
			texture->addRef();
			m_textureRegions.push_back(new TextureRegion(texture,
				Vector2(float(x)/nColumns, float(y)/nRows),
				Vector2(float(x+1)/nColumns, float(y+1)/nRows)));
		}
	}
	texture->release();
}

Animation::~Animation()
{
	for(int i = 0; i < m_textureRegions.size(); i++) {
		m_textureRegions[i]->release();
	}
}

TextureRegion *Animation::getKeyFrame(int frameIndex)
{
	TextureRegion *region = m_textureRegions[frameIndex];
	region->addRef();
	return region;
}