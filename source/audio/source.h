#ifndef X2D_AUDIO_SOURCE_H
#define X2D_AUDIO_SOURCE_H

#include "common/engine.h"

class XAudioSource
{
public:
	AS_DECL_REF

	virtual ~XAudioSource() {}

	virtual void play() { NOT_IMPLEMENTED(play) }
	virtual void stop() { NOT_IMPLEMENTED(stop) }
	virtual void playOnce() { setLooping(false); play(); }

	virtual bool isPlaying() const { NOT_IMPLEMENTED_RET(isPlaying, false) }

	virtual void setPosition(const Vector2&)	{ NOT_IMPLEMENTED(setPosition) }
	virtual Vector2 getPosition() const			{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }
	virtual void setVelocity(const Vector2&)	{ NOT_IMPLEMENTED(setVelocity) }
	virtual Vector2 getVelocity() const			{ NOT_IMPLEMENTED_RET(getVelocity, Vector2(0.0f)) }
	virtual void setLooping(const bool)			{ NOT_IMPLEMENTED(setLooping) }
	virtual bool getLooping() const				{ NOT_IMPLEMENTED_RET(getLooping, false) }
	virtual void setGain(const float)			{ NOT_IMPLEMENTED(setGain) }
	virtual float getGain() const				{ NOT_IMPLEMENTED_RET(getGain, 0.0f) }
	virtual void setPitch(const float)			{ NOT_IMPLEMENTED(setPitch) }
	virtual float getPitch() const				{ NOT_IMPLEMENTED_RET(getPitch, 0.0f) }
};

#endif // X2D_AUDIO_SOURCE_H