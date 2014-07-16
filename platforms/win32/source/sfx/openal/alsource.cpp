#include "alsource.h"
#include "alaudio.h"

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

ALsource::ALsource(AudioBuffer *buffer)
{
	// Create source object
	alGenSources(1, &m_sourceId); AL_ASSERT

	// Create buffer object
	alGenBuffers(1, &m_bufferId); AL_ASSERT
	alBufferData(m_bufferId, toALFormat(buffer->getFormat()), (ALvoid*)buffer->getData(), (ALsizei)buffer->getSize(), (ALsizei)buffer->getFrequency()); AL_ASSERT

	// Set source buffer
	alSourcei(m_sourceId, AL_BUFFER, m_bufferId); AL_ASSERT
}

ALsource::~ALsource()
{
	alDeleteSources(1, &m_sourceId);
	alDeleteBuffers(1, &m_bufferId);
}

void ALsource::play()
{
	alSourcePlay(m_sourceId); AL_ASSERT
}

void ALsource::stop()
{
	alSourceStop(m_sourceId);
}

bool ALsource::isPlaying() const
{
	ALenum state;
	alGetSourcei(m_sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void ALsource::setPosition(const Vector2 &position)
{
	alSource3f(m_sourceId, AL_POSITION, position.x, position.y, 0.0f);
}

Vector2 ALsource::getPosition() const
{
	Vector2 position(0.0f);
	alGetSource3f(m_sourceId, AL_POSITION, &position.x, &position.y, 0);
	return position;
}

void ALsource::setVelocity(const Vector2 &velocity)
{
	alSource3f(m_sourceId, AL_VELOCITY, velocity.x, velocity.y, 0.0f);
}

Vector2 ALsource::getVelocity() const
{
	Vector2 velocity(0.0f);
	alGetSource3f(m_sourceId, AL_VELOCITY, &velocity.x, &velocity.y, 0);
	return velocity;
}

void ALsource::setLooping(const bool looping)
{
	alSourcei(m_sourceId, AL_LOOPING, looping);
}

bool ALsource::getLooping() const
{
	ALint looping;
	alGetSourcei(m_sourceId, AL_LOOPING, &looping);
	return looping != 0;
}

void ALsource::setGain(const float gain)
{
	alSourcef(m_sourceId, AL_GAIN, gain);
}

float ALsource::getGain() const
{
	ALfloat gain;
	alGetSourcef(m_sourceId, AL_GAIN, &gain);
	return gain;
}

void ALsource::setPitch(const float pitch)
{
	alSourcef(m_sourceId, AL_PITCH, pitch);
}

float ALsource::getPitch() const
{
	ALfloat pitch;
	alGetSourcef(m_sourceId, AL_PITCH, &pitch);
	return pitch;
}

/*void OpenALManager::setSourceMinDist(const int sourceId, const float dist)
{
	alSourcef(sourceId, AL_REFERENCE_DISTANCE, dist);
}

void OpenALManager::setSourceMaxDist(const int sourceId, const float dist)
{
	alSourcef(sourceId, AL_MAX_DISTANCE, dist);
}*/