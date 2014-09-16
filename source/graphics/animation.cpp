#include "animation.h"
#include "texture.h"
#include "textureRegion.h"

AS_REG_REF(XAnimation)

int XAnimation::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("XAnimation", asBEHAVE_FACTORY, "XAnimation @f(XTexture@, const int, const int)", asFUNCTIONPR(Factory, (XTexture*, const int, const int), XAnimation*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XAnimation", "XTextureRegion getKeyFrame(int frameIndex) const", asMETHOD(XAnimation, getKeyFrame), asCALL_THISCALL); AS_ASSERT

	return r;
}

XAnimation::XAnimation(XTexture *texture, const int nRows, const int nColumns)
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
				XTextureRegion(texture,
					Vector2(float(x)/nColumns, float(y)/nRows),
					Vector2(float(x+1)/nColumns, float(y+1)/nRows))
				);
		}
	}

	// Release argument ref
	texture->release();
}

XAnimation::~XAnimation()
{
	m_textureRegions.clear();
}

XTextureRegion XAnimation::getKeyFrame(int frameIndex)
{
	return m_textureRegions[frameIndex];
}