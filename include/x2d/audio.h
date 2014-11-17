#ifndef X2D_AUDIO_H
#define X2D_AUDIO_H

#include "engine.h"

#include <al.h>
#include <alc.h>

#ifdef X2D_DEBUG
	#define AL_ASSERT \
		{ \
			ALenum error = alGetError(); \
			if(error != AL_NO_ERROR) { \
				LOG("OpenAL error: '%X'", error); \
			} \
		}
#else
	#define AL_ASSERT
#endif

/*********************************************************************
**	Abstract Audio Manager											**
**********************************************************************/

class XAudioSource;
class XAudioBuffer;

class XDAPI XAudioManager
{
	SINGLETON_DECL(XAudioManager)

	friend class XEngine;

public:
	XAudioManager();
	~XAudioManager();

	// Listener position
	void setPosition(const Vector2 &position);
	Vector2 getPosition() const;

	// Listener velocity
	void setVelocity(const Vector2&);
	Vector2 getVelocity() const;

	// Listener orientation
	void setOrientation(const Vector3&);
	Vector3 getOrientation() const;
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
class XDAPI XAudioSource
{
public:
	XAudioSource(XAudioBuffer *buffer);
	~XAudioSource();

	void play();
	void stop();
	void playOnce() { setLooping(false); play(); }

	bool isPlaying() const;

	void setPosition(const Vector2&);
	Vector2 getPosition() const;
	void setVelocity(const Vector2&);
	Vector2 getVelocity() const;
	void setLooping(const bool);
	bool getLooping() const;
	void setGain(const float);
	float getGain() const;
	void setPitch(const float);
	float getPitch() const;

private:
	ALuint m_sourceId;
	ALuint m_bufferId;
};

#endif // X2D_AUDIO_H