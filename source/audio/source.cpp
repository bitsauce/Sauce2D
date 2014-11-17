//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/audio.h>

ALenum toALFormat(XAudioBuffer::Format format)
{
	switch(format)
	{
	case XAudioBuffer::STEREO16: return AL_FORMAT_STEREO16;
	case XAudioBuffer::STEREO8: return AL_FORMAT_STEREO8;
	case XAudioBuffer::MONO16: return AL_FORMAT_MONO16;
	case XAudioBuffer::MONO8: return AL_FORMAT_MONO8;
	}
	return 0;
}

XAudioSource::XAudioSource(XAudioBuffer *buffer)
{
	// Create source object
	alGenSources(1, &m_sourceId); AL_ASSERT

	// Create buffer object
	alGenBuffers(1, &m_bufferId); AL_ASSERT
	alBufferData(m_bufferId, toALFormat(buffer->getFormat()), (ALvoid*)buffer->getData(), (ALsizei)buffer->getSize(), (ALsizei)buffer->getFrequency()); AL_ASSERT

	// Set source buffer
	alSourcei(m_sourceId, AL_BUFFER, m_bufferId); AL_ASSERT
}

XAudioSource::~XAudioSource()
{
	alDeleteSources(1, &m_sourceId);
	alDeleteBuffers(1, &m_bufferId);
}

void XAudioSource::play()
{
	alSourcePlay(m_sourceId); AL_ASSERT
}

void XAudioSource::stop()
{
	alSourceStop(m_sourceId);
}

bool XAudioSource::isPlaying() const
{
	ALenum state;
	alGetSourcei(m_sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void XAudioSource::setPosition(const Vector2 &position)
{
	alSource3f(m_sourceId, AL_POSITION, position.x, position.y, 0.0f);
}

Vector2 XAudioSource::getPosition() const
{
	Vector2 position(0.0f);
	alGetSource3f(m_sourceId, AL_POSITION, &position.x, &position.y, 0);
	return position;
}

void XAudioSource::setVelocity(const Vector2 &velocity)
{
	alSource3f(m_sourceId, AL_VELOCITY, velocity.x, velocity.y, 0.0f);
}

Vector2 XAudioSource::getVelocity() const
{
	Vector2 velocity(0.0f);
	alGetSource3f(m_sourceId, AL_VELOCITY, &velocity.x, &velocity.y, 0);
	return velocity;
}

void XAudioSource::setLooping(const bool looping)
{
	alSourcei(m_sourceId, AL_LOOPING, looping);
}

bool XAudioSource::getLooping() const
{
	ALint looping;
	alGetSourcei(m_sourceId, AL_LOOPING, &looping);
	return looping != 0;
}

void XAudioSource::setGain(const float gain)
{
	alSourcef(m_sourceId, AL_GAIN, gain);
}

float XAudioSource::getGain() const
{
	ALfloat gain;
	alGetSourcef(m_sourceId, AL_GAIN, &gain);
	return gain;
}

void XAudioSource::setPitch(const float pitch)
{
	alSourcef(m_sourceId, AL_PITCH, pitch);
}

float XAudioSource::getPitch() const
{
	ALfloat pitch;
	alGetSourcef(m_sourceId, AL_PITCH, &pitch);
	return pitch;
}

/*void OpenALManager::setSourceMinDist(const int sourceId, const float dist)
{
	XAudioSourcef(sourceId, AL_REFERENCE_DISTANCE, dist);
}

void OpenALManager::setSourceMaxDist(const int sourceId, const float dist)
{
	XAudioSourcef(sourceId, AL_MAX_DISTANCE, dist);
}*/