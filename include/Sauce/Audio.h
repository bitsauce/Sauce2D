#ifndef SAUCE_AUDIO_H
#define SAUCE_AUDIO_H

#include <Sauce/Common.h>

/*#include "../3rdparty/openal/include/al.h"
#include "../3rdparty/openal/include/alc.h"

#ifdef SAUCE_DEBUG
	#define AL_ASSERT \
		{ \
			ALenum error = alGetError(); \
			if(error != AL_NO_ERROR) { \
				LOG("OpenAL error: '%X'", error); \
			} \
		}
#else
	#define AL_ASSERT
#endif*/

/*********************************************************************
**	Abstract Audio Manager											**
**********************************************************************/

BEGIN_SAUCE_NAMESPACE
/*
class AudioSource;
class AudioBuffer;

class SAUCE_API AudioManager
{
	friend class Game;
public:
	AudioManager();
	~AudioManager();

	// Listener position
	void setPosition(const Vector2F &position);
	Vector2F getPosition() const;

	// Listener velocity
	void setVelocity(const Vector2&);
	Vector2F getVelocity() const;

	// Listener orientation
	void setOrientation(const Vector3&);
	Vector3 getOrientation() const;

private:
	static AudioManager *s_this;
};*/

/*********************************************************************
**	Audio buffer													**
**********************************************************************/
/*class SAUCE_API AudioBuffer
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
	AudioBuffer(void *data, uint size, uint frequency, Format format);
	~AudioBuffer();

	Format getFormat() const;
	void *getData() const;
	uint getSize() const;
	uint getFrequency() const;

private:
	Format m_format;
	void *m_data;
	uint m_size;
	uint m_frequency;
};*/

/*********************************************************************
**	Audio source													**
**********************************************************************/
/*class SAUCE_API AudioSource
{
public:
	AudioSource(AudioBuffer *buffer);
	~AudioSource();

	void play();
	void stop();
	void playOnce() { setLooping(false); play(); }

	bool isPlaying() const;

	void setPosition(const Vector2&);
	Vector2F getPosition() const;
	void setVelocity(const Vector2&);
	Vector2F getVelocity() const;
	void setLooping(const bool);
	bool getLooping() const;
	void setGain(const float);
	float getGain() const;
	void setPitch(const float);
	float getPitch() const;

private:
	ALuint m_sourceId;
	ALuint m_bufferId;
};*/

END_SAUCE_NAMESPACE

#endif // SAUCE_AUDIO_H
