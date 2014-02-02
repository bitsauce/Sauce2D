#ifndef SFX_OPENAL_H
#define SFX_OPENAL_H

#include "x2d/sfx.h"

#include <al.h>
#include <alc.h>

// OpenAL sfx manager
class OpenALManager : public X2DSound
{
public:
	~OpenALManager();

	// Initialation
	void init();

	// Sfx source
	int createSource();
	void deleteSource(const int sourceId);

	void playSource(const int sourceId);
	void stopSource(const int sourceId);
	
	void setSourceBuffer(const int sourceId, const int bufferId);
	void setSourcePosition(const int sourceId, const float x, const float y, const float z);
	void setSourceVelocity(const int sourceId, const float x, const float y, const float z);
	void setSourceLooping(const int sourceId, const bool looping);
	void setSourceGain(const int sourceId, const float gain);
	void setSourcePitch(const int sourceId, const float pitch);
	void setSourceMinDist(const int sourceId, const float dist);
	void setSourceMaxDist(const int sourceId, const float dist);

	// Sfx listener
	void setListenerPosition(const float x, const float y, const float z);
	void setListenerVelocity(const float x, const float y, const float z);
	void setListenerOrientation(const float x, const float y, const float z);

	// Sfx buffer
	int loadFile(const string &filePath);
	void deleteBuffer(const int bufferId);
};

#endif // SFX_OPENAL_H