//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/audio.h>

BEGIN_CGF_NAMESPACE
/*
ALenum toALFormat(AudioBuffer::Format format)
{
	switch(format)
	{
	case AudioBuffer::STEREO16: return AL_FORMAT_STEREO16;
	case AudioBuffer::STEREO8: return AL_FORMAT_STEREO8;
	case AudioBuffer::MONO16: return AL_FORMAT_MONO16;
	case AudioBuffer::MONO8: return AL_FORMAT_MONO8;
	}
	return 0;
}

AudioSource::AudioSource(AudioBuffer *buffer)
{
	// Create source object
	alGenSources(1, &m_sourceId); AL_ASSERT

	// Create buffer object
	alGenBuffers(1, &m_bufferId); AL_ASSERT
	alBufferData(m_bufferId, toALFormat(buffer->getFormat()), (ALvoid*)buffer->getData(), (ALsizei)buffer->getSize(), (ALsizei)buffer->getFrequency()); AL_ASSERT

	// Set source buffer
	alSourcei(m_sourceId, AL_BUFFER, m_bufferId); AL_ASSERT
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &m_sourceId);
	alDeleteBuffers(1, &m_bufferId);
}

void AudioSource::play()
{
	alSourcePlay(m_sourceId); AL_ASSERT
}

void AudioSource::stop()
{
	alSourceStop(m_sourceId);
}

bool AudioSource::isPlaying() const
{
	ALenum state;
	alGetSourcei(m_sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void AudioSource::setPosition(const Vector2 &position)
{
	alSource3f(m_sourceId, AL_POSITION, position.x, position.y, 0.0f);
}

Vector2 AudioSource::getPosition() const
{
	Vector2 position(0.0f);
	alGetSource3f(m_sourceId, AL_POSITION, &position.x, &position.y, 0);
	return position;
}

void AudioSource::setVelocity(const Vector2 &velocity)
{
	alSource3f(m_sourceId, AL_VELOCITY, velocity.x, velocity.y, 0.0f);
}

Vector2 AudioSource::getVelocity() const
{
	Vector2 velocity(0.0f);
	alGetSource3f(m_sourceId, AL_VELOCITY, &velocity.x, &velocity.y, 0);
	return velocity;
}

void AudioSource::setLooping(const bool looping)
{
	alSourcei(m_sourceId, AL_LOOPING, looping);
}

bool AudioSource::getLooping() const
{
	ALint looping;
	alGetSourcei(m_sourceId, AL_LOOPING, &looping);
	return looping != 0;
}

void AudioSource::setGain(const float gain)
{
	alSourcef(m_sourceId, AL_GAIN, gain);
}

float AudioSource::getGain() const
{
	ALfloat gain;
	alGetSourcef(m_sourceId, AL_GAIN, &gain);
	return gain;
}

void AudioSource::setPitch(const float pitch)
{
	alSourcef(m_sourceId, AL_PITCH, pitch);
}

float AudioSource::getPitch() const
{
	ALfloat pitch;
	alGetSourcef(m_sourceId, AL_PITCH, &pitch);
	return pitch;
}

/*void OpenALManager::setSourceMinDist(const int sourceId, const float dist)
{
	AudioSourcef(sourceId, AL_REFERENCE_DISTANCE, dist);
}

void OpenALManager::setSourceMaxDist(const int sourceId, const float dist)
{
	AudioSourcef(sourceId, AL_MAX_DISTANCE, dist);
}*/

END_CGF_NAMESPACE
