#ifndef X2D_SOUND_H
#define X2D_SOUND_H

#include <x2d/config.h>
#include <x2d/console.h>

#include <x2d/audio/source.h>
#include <x2d/audio/buffer.h>

/*********************************************************************
**	Abstract Sound Manager											**
**********************************************************************/
class XDAPI xdAudio
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON
		
	virtual void setPosition(const Vector2&)	{ NOT_IMPLEMENTED(setPosition) }
	virtual Vector2 getPosition() const			{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }
	virtual void setVelocity(const Vector2&)	{ NOT_IMPLEMENTED(setVelocity) }
	virtual Vector2 getVelocity() const			{ NOT_IMPLEMENTED_RET(getVelocity, Vector2(0.0f)) }
	virtual void setOrientation(const Vector3&) { NOT_IMPLEMENTED(setOrientation) }
	virtual Vector3 getOrientation() const		{ NOT_IMPLEMENTED_RET(getOrientation, Vector3(0.0f)) }
	
	static AudioSource *CreateSource(AudioBuffer *buffer);
	static AudioSource *CreateSource(const string &path);

private:
	static xdAudio *s_this;

	virtual AudioSource *createSource(AudioBuffer *buffer) = 0;
};

#endif // X2D_SOUND_H