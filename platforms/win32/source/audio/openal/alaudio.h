#ifndef WIN_OPENAL_H
#define WIN_OPENAL_H

#include <x2d/audio.h>

#include <al.h>
#include <alc.h>

// OpenAL sfx manager
class OpenAL : public XAudioManager
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

	XAudioSource *createSource(XAudioBuffer *buffer);
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

#endif // WIN_OPENAL_H