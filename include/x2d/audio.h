#ifndef X2D_AUDIO_H
#define X2D_AUDIO_H

#include "engine.h"

/*********************************************************************
**	Abstract Audio Manager											**
**********************************************************************/

class XAudioSource;
class XAudioBuffer;

class XDAPI XAudioManager
{
	friend class XEngine;
public:
	// Listener position
	virtual void setPosition(const Vector2&)	{ NOT_IMPLEMENTED(setPosition) }
	virtual Vector2 getPosition() const			{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Listener velocity
	virtual void setVelocity(const Vector2&)	{ NOT_IMPLEMENTED(setVelocity) }
	virtual Vector2 getVelocity() const			{ NOT_IMPLEMENTED_RET(getVelocity, Vector2(0.0f)) }

	// Listener orientation
	virtual void setOrientation(const Vector3&) { NOT_IMPLEMENTED(setOrientation) }
	virtual Vector3 getOrientation() const		{ NOT_IMPLEMENTED_RET(getOrientation, Vector3(0.0f)) }
	
	// Global factories
	static XAudioSource *CreateSource(XAudioBuffer *buffer);
	static XAudioSource *CreateSource(const string &path);

private:
	// XAudioManager instance
	static XAudioManager *s_this;

	// Virtual factory functions
	virtual XAudioSource *createSource(XAudioBuffer *buffer) = 0;
};

/*********************************************************************
**	Audio buffer													**
**********************************************************************/
class XDAPI XAudioBuffer
{
public:
	
	enum Format
	{
		STEREO16,
		STEREO8,
		MONO16,
		MONO8
	};

public:
	XAudioBuffer(void *data, uint size, uint frequency, Format format);
	~XAudioBuffer();

	Format getFormat() const;
	void *getData() const;
	uint getSize() const;
	uint getFrequency() const;

private:
	Format m_format;
	void *m_data;
	uint m_size;
	uint m_frequency;
};

/*********************************************************************
**	Audio source													**
**********************************************************************/
class XAudioSource
{
public:
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

#endif // X2D_AUDIO_H