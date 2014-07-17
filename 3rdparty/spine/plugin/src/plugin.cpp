#include "plugin.h"
#include "skeleton.h"
#include "animation.h"
#include "event.h"

#include <x2d/scriptengine.h>
#include <x2d/util.h>
#include <x2d/graphics.h>
#include <x2d/graphics/batch.h>
#include <x2d/graphics/texture.h>
#include <x2d/filesystem.h>

#include <spine/spine.h>
#include <spine/extension.h>

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	Texture *texture = xdGraphics::CreateTexture(path);
	if(texture)
	{
		self->rendererObject = texture;
		self->width = texture->getWidth();
		self->height = texture->getHeight();
	}
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	if(self->rendererObject) {
		((Texture*)self->rendererObject)->release();
	}
}

char* _spUtil_readFile(const char* path, int* length)
{
	string content;
	char *data = 0;
	if(xdFileSystem::ReadFile(path, content))
	{
		*length = content.size();
		data = MALLOC(char, *length);
		memcpy(data, content.c_str(), *length);
	}
	return data;
}

int CreatePlugin(xdScriptEngine *scriptEngine)
{
	int r = 0;

	spBone_setYDown(true);
	
	r = scriptEngine->registerRefType("spSkeleton", asMETHOD(spSkeletonWrapper, addRef), asMETHOD(spSkeletonWrapper, release));
	r = scriptEngine->registerRefType("spAnimation", asMETHOD(spAnimationWrapper, addRef), asMETHOD(spAnimationWrapper, release));
	r = scriptEngine->registerRefType("spAnimationStateData", asMETHOD(spAnimationStateDataWrapper, addRef), asMETHOD(spAnimationStateDataWrapper, release));
	r = scriptEngine->registerRefType("spAnimationState", asMETHOD(spAnimationStateWrapper, addRef), asMETHOD(spAnimationStateWrapper, release));
	r = scriptEngine->registerRefType("spEvent", asMETHOD(spEventWrapper, addRef), asMETHOD(spEventWrapper, release));

	r = scriptEngine->registerFuncdef("void spEventCallback(spEvent@)");

	r = scriptEngine->registerObjectFactory("spSkeleton", "spSkeleton @f(const string &in, const string &in, const float scale = 1.0f)", asFUNCTION(spSkeletonWrapper::Factory));
	r = scriptEngine->registerObjectMethod("spSkeleton", "spAnimation @findAnimation(const string &in)", asMETHOD(spSkeletonWrapper, findAnimation));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void set_position(const Vector2 &in)", asMETHOD(spSkeletonWrapper, setPosition));
	r = scriptEngine->registerObjectMethod("spSkeleton", "Vector2 get_position() const", asMETHOD(spSkeletonWrapper, getPosition));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void set_flipX(const bool)", asMETHOD(spSkeletonWrapper, setFlipX));
	r = scriptEngine->registerObjectMethod("spSkeleton", "bool get_flipX() const", asMETHOD(spSkeletonWrapper, getFlipX));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void set_flipY(const bool)", asMETHOD(spSkeletonWrapper, setFlipY));
	r = scriptEngine->registerObjectMethod("spSkeleton", "bool get_flipY() const", asMETHOD(spSkeletonWrapper, getFlipY));
	r = scriptEngine->registerObjectMethod("spSkeleton", "Texture @get_texture() const", asMETHOD(spSkeletonWrapper, getTexture));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void draw(Batch@)", asMETHOD(spSkeletonWrapper, draw));
	
	r = scriptEngine->registerObjectMethod("spAnimation", "void set_time(const float)", asMETHOD(spAnimationWrapper, setTime));
	r = scriptEngine->registerObjectMethod("spAnimation", "float get_time() const", asMETHOD(spAnimationWrapper, getTime));
	r = scriptEngine->registerObjectMethod("spAnimation", "void set_looping(const bool)", asMETHOD(spAnimationWrapper, setLooping));
	r = scriptEngine->registerObjectMethod("spAnimation", "bool get_looping() const", asMETHOD(spAnimationWrapper, getLooping));
	r = scriptEngine->registerObjectMethod("spAnimation", "void apply(const float)", asMETHOD(spAnimationWrapper, apply));
	r = scriptEngine->registerObjectMethod("spAnimation", "void mix(const float, const float)", asMETHOD(spAnimationWrapper, mix));

	r = scriptEngine->registerEnum("spEventType");
	r = scriptEngine->registerEnumValue("spEventType", "SP_ANIMATION_START", SP_ANIMATION_START);
	r = scriptEngine->registerEnumValue("spEventType", "SP_ANIMATION_END", SP_ANIMATION_END);
	r = scriptEngine->registerEnumValue("spEventType", "SP_ANIMATION_COMPLETE", SP_ANIMATION_COMPLETE);
	r = scriptEngine->registerEnumValue("spEventType", "SP_ANIMATION_EVENT", SP_ANIMATION_EVENT);
	
	r = scriptEngine->registerObjectMethod("spEvent", "string get_string() const", asMETHOD(spEventWrapper, getString));
	r = scriptEngine->registerObjectMethod("spEvent", "int get_int() const", asMETHOD(spEventWrapper, getInt));
	r = scriptEngine->registerObjectMethod("spEvent", "float get_float() const", asMETHOD(spEventWrapper, getFloat));
	r = scriptEngine->registerObjectMethod("spEvent", "spEventType get_type() const", asMETHOD(spEventWrapper, getType));
	r = scriptEngine->registerObjectMethod("spEvent", "int get_loopCount() const", asMETHOD(spEventWrapper, getLoopCount));
	
	r = scriptEngine->registerObjectFactory("spAnimationStateData", "spAnimationStateData @f(spSkeleton@)", asFUNCTION(spAnimationStateDataWrapper::Factory));
	r = scriptEngine->registerObjectMethod("spAnimationStateData", "void getMix(spAnimation@, spAnimation@) const", asMETHOD(spAnimationStateDataWrapper, getMix));
	r = scriptEngine->registerObjectMethod("spAnimationStateData", "void setMix(spAnimation@, spAnimation@, const float)", asMETHODPR(spAnimationStateDataWrapper, setMix, (spAnimationWrapper*, spAnimationWrapper*, const float), void));
	r = scriptEngine->registerObjectMethod("spAnimationStateData", "void setMix(const string &in, const string &in, const float)", asMETHODPR(spAnimationStateDataWrapper, setMix, (const string&, const string&, const float), void));

	r = scriptEngine->registerObjectFactory("spAnimationState", "spAnimationState @f(spAnimationStateData@)", asFUNCTION(spAnimationStateWrapper::Factory));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void set_looping(const bool)", asMETHOD(spAnimationStateWrapper, setLooping));
	r = scriptEngine->registerObjectMethod("spAnimationState", "bool get_looping() const", asMETHOD(spAnimationStateWrapper, getLooping));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void set_timeScale(const float)", asMETHOD(spAnimationStateWrapper, setTimeScale));
	r = scriptEngine->registerObjectMethod("spAnimationState", "float get_timeScale() const", asMETHOD(spAnimationStateWrapper, getTimeScale));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void set_eventCallback(spEventCallback@)", asMETHOD(spAnimationStateWrapper, setEventCallback));
	r = scriptEngine->registerObjectMethod("spAnimationState", "spEventCallback@ get_eventCallback() const", asMETHOD(spAnimationStateWrapper, getEventCallback));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void setAnimation(spAnimation@)", asMETHODPR(spAnimationStateWrapper, setAnimation, (spAnimationWrapper*), void));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void setAnimation(const string &in)", asMETHODPR(spAnimationStateWrapper, setAnimation, (const string&), void));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void addAnimation(spAnimation@, const float)", asMETHODPR(spAnimationStateWrapper, addAnimation, (spAnimationWrapper*, const float), void));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void addAnimation(const string &in, const float)", asMETHODPR(spAnimationStateWrapper, addAnimation, (const string&, const float), void));
	r = scriptEngine->registerObjectMethod("spAnimationState", "void update(const float)", asMETHOD(spAnimationStateWrapper, update));

	return 0;
}

void ReleasePlugin()
{
}