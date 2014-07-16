#ifndef SFX_ALSOURCE_H
#define SFX_ALSOURCE_H

#include "alaudio.h"

class ALsource : public AudioSource
{
public:
	ALsource(AudioBuffer *buffer);
	~ALsource();

	void play();
	void stop();

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

#endif // SFX_ALSOURCE_H