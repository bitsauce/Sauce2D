#ifndef SFX_OPENAL_H
#define SFX_OPENAL_H

#include <x2d/audio.h>

#include <al.h>
#include <alc.h>

// OpenAL sfx manager
class OpenAL : public xdAudio
{
public:
	OpenAL();
	~OpenAL();

	void setPosition(const Vector2&);
	Vector2 getPosition() const;
	void setVelocity(const Vector2&);
	Vector2 getVelocity() const;
	void setOrientation(const Vector3&);
	Vector3 getOrientation() const;

private:

	AudioSource *createSource(AudioBuffer *buffer);
};

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

#endif // SFX_OPENAL_H