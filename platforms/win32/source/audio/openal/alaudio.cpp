//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "alaudio.h"
#include "alsource.h"

OpenAL::OpenAL()
{
	// Get default device
	const ALCchar *defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	//LOG("** Initializing OpenAL: Using default device '%s' **", defaultDevice);

	// Open device
	ALCdevice *device;
	if((device = alcOpenDevice(defaultDevice)) == NULL) {
		// Failed
		//ERR("Error: failed to open sound device\n");
		return;
	}

	// Create context
	ALCcontext *ctx = alcCreateContext(device, NULL);

	// Set active context
	alcMakeContextCurrent(ctx);

	// Setup listner
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	//alListenerfv(AL_ORIENTATION, 0.0f, 0.0f, 1.0f);
}

OpenAL::~OpenAL()
{
	// Get active context and device
	ALCcontext *ctx = alcGetCurrentContext();
	ALCdevice *device = alcGetContextsDevice(ctx);

	// Disable and release context
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);

	// Close device
	alcCloseDevice(device);
}

XAudioSource *OpenAL::createSource(XAudioBuffer *buffer)
{
	return new ALsource(buffer);
}

void OpenAL::setPosition(const Vector2 &position)
{
	alListener3f(AL_POSITION, position.x, position.y, 0.0f);
}

Vector2 OpenAL::getPosition() const
{
	Vector2 position(0.0f);
	alGetListener3f(AL_POSITION, &position.x, &position.y, 0);
	return position;
}

void OpenAL::setVelocity(const Vector2 &velocity)
{
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, 0.0f);
}

Vector2 OpenAL::getVelocity() const
{
	Vector2 velocity(0.0f);
	alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, 0);
	return velocity;
}

void OpenAL::setOrientation(const Vector3 &axis)
{
	//alListener3f(AL_ORIENTATION, axis.x, axis.y, axis.z);
}

Vector3 OpenAL::getOrientation() const
{
	Vector3 axis(0.0f);
	//alGetListener3f(AL_ORIENTATION, &axis.x, &axis.y, &axis.z);
	return axis;
}