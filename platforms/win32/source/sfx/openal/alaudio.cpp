//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
#ifdef NO
#include "openal.h"

#include "x2d/assetloader.h"
#include "x2d/engine.h"

#define WAV_LOAD_ERROR 0

 // buffer is always in big endian!
static unsigned short readByte16(const unsigned char buffer[2])
{
	#if PCCE_BIG_ENDIAN
		return (buffer[0] << 8) + buffer[1];
	#else
		return (buffer[1] << 8) + buffer[0];
	#endif
}
static unsigned long readByte32(const unsigned char buffer[4])
{
	#if PCCE_BIG_ENDIAN
		return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
	#else
		return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
	#endif
}

//-----------------------------------------

//  References:
//  -  http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
//  -  http://www.borg.com/~jglatt/tech/wave.htm
//  -  Alut source code: static BufferData *loadWavFile (InputStream *stream)
//     http://www.openal.org/repos/openal/tags/freealut_1_1_0/alut/alut/src/alutLoader.c

ALuint loadSound(const string& fileName)
{
	const unsigned int BUFFER_SIZE = 32768;     // 32 KB buffers
	long bytes;
	vector <char> data;
	ALenum format;
	ALsizei freq;

	// Local resources
	FILE *f = NULL;
	char *array = NULL;
	ALuint buffer = AL_NONE;

	alGetError();

	// Main process
	string filePath = fileName;
	//gameEngine->assetLoader->transformFilePath(filePath);

	// Open for binary reading
	fopen_s(&f, filePath.c_str(), "rb");
	if(!f)
	{
		ERR("Could not read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// buffers
	char magic[5];
	magic[4] = '\0';
	unsigned char buffer32[4];
	unsigned char buffer16[2];

	// check magic
	if(fread(magic, 4, 1, f) != 1)
	{
		ERR("Wav magic failed '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(std::string(magic) != "RIFF")
	{
		ERR("Wrong wav file format. This file is not a .wav file (no RIFF magic) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// Skip 4 bytes (file size)
	fseek(f,4,SEEK_CUR);

	// Check file format
	if(fread(magic, 4, 1, f) != 1)
	{
		ERR("More wav magic failed '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(string(magic) != "WAVE")
	{
		ERR("Wrong wav file format. This file is not a .wav file (no WAVE magic) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// check 'fmt ' sub chunk (1)
	if(fread(magic,4,1,f) != 1)
	{
		ERR("Failed to read subchunk '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(string(magic) != "fmt ")
	{
		ERR("Wrong wav file format. This file is not a .wav file (no 'fmt' subchunk) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// read (1)'s size
	if(fread(buffer32, 4, 1, f) != 1)
	{
		ERR("Failed to read subchunk size '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	unsigned long subChunk1Size = readByte32(buffer32);
	if(!(subChunk1Size >= 16))
	{
		ERR("Wrong wav file format. This file is not a .wav file ('fmt ' chunk too small, truncated file?) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// check PCM audio format
	if(fread(buffer16,2,1,f) != 1)
	{
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	unsigned short audioFormat = readByte16(buffer16);
	if(audioFormat != 1)
	{
		ERR("Wrong wav file format. This file is not a .wav file (audio format is not PCM) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// read number of channels
	if(fread(buffer16, 2, 1, f) != 1)
	{
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned short channels = readByte16(buffer16);

	// read frequency (sample rate)
	if(fread(buffer32, 4, 1, f) != 1)
	{
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned long frequency = readByte32(buffer32);

	// skip 6 bytes (Byte rate (4), Block align (2))
	fseek(f, 6, SEEK_CUR);

	// read bits per sample
	if(fread(buffer16, 2, 1, f) != 1)
	{
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned short bps = readByte16(buffer16);

	if(channels == 1)
		format = (bps == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	else
		format = (bps == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

	// check 'data' sub chunk (2)
	if(fread(magic, 4, 1, f) != 1)
	{ 
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	if(std::string(magic) != "data")
	{
		ERR("Wrong wav file format. This file is not a .wav file (no data subchunk) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(fread(buffer32,4,1,f) != 1)
	{
		ERR("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned long subChunk2Size = readByte32(buffer32);

	// The frequency of the sampling rate
	freq = frequency;
	if(sizeof(freq) != sizeof(frequency))
	{
		ERR("LoadWav: freq and frequency different sizes");
		return WAV_LOAD_ERROR;
	}

	array = new char[BUFFER_SIZE];

	while (data.size() != subChunk2Size)
	{
		// Read up to a buffer's worth of decoded sound data
		bytes = fread(array, 1, BUFFER_SIZE, f);

		if(bytes <= 0)
			break;

		if (data.size() + bytes > subChunk2Size)
		bytes = subChunk2Size - data.size();

		// Append to end of buffer
		data.insert(data.end(), array, array + bytes);
	};

	delete []array;
	array = NULL;

	fclose(f);
	f = NULL;

	alGenBuffers(1, &buffer);
	if(alGetError() != AL_NO_ERROR)
	{
		ERR("LoadWav: Could not generate buffer");
		return WAV_LOAD_ERROR;
	}

	if(AL_NONE == buffer)
	{
		ERR("LoadWav: Could not generate buffer");
		return WAV_LOAD_ERROR;
	}

	alBufferData(buffer, format, &data[0], data.size(), freq);
	if(alGetError() != AL_NO_ERROR)
	{
		ERR("LoadWav: Could not load buffer data");
		return WAV_LOAD_ERROR;
	}

	return buffer;
}

OpenALManager::~OpenALManager()
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

void OpenALManager::init()
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

	// Clear Error Code
	alGetError();

	// Setup listner
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_ORIENTATION, 0.0f, 0.0f, 0.0f);
}

int OpenALManager::createSource()
{
	// Create a sound source and return its id
	uint sourceId;
	alGenSources(1, &sourceId);
	return (int)sourceId;
}

void OpenALManager::deleteSource(const int sourceId)
{
	if(sourceId < 0) return;
	uint usourceId = sourceId;
	alDeleteSources(1, &usourceId);
}

void OpenALManager::playSource(const int sourceId)
{
	alSourcePlay(sourceId);
}

void OpenALManager::stopSource(const int sourceId)
{
	alSourceStop(sourceId);
}

void OpenALManager::setSourceBuffer(const int sourceId, const int bufferId)
{
	alSourcei(sourceId, AL_BUFFER, bufferId);
}

void OpenALManager::setSourcePosition(const int sourceId, const float x, const float y, const float z)
{
	alSource3f(sourceId, AL_POSITION, x, y, z);
}

void OpenALManager::setSourceVelocity(const int sourceId, const float x, const float y, const float z)
{
	alSource3f(sourceId, AL_VELOCITY, x, y, z);
}

void OpenALManager::setSourceLooping(const int sourceId, const bool looping)
{
	alSourcei(sourceId, AL_LOOPING, looping);
}

void OpenALManager::setSourceGain(const int sourceId, const float gain)
{
	alSourcef(sourceId, AL_GAIN, gain);
}

void OpenALManager::setSourcePitch(const int sourceId, const float pitch)
{
	alSourcef(sourceId, AL_PITCH, pitch);
}

void OpenALManager::setSourceMinDist(const int sourceId, const float dist)
{
	alSourcef(sourceId, AL_REFERENCE_DISTANCE, dist);
}

void OpenALManager::setSourceMaxDist(const int sourceId, const float dist)
{
	alSourcef(sourceId, AL_MAX_DISTANCE, dist);
}

void OpenALManager::setListenerPosition(const float x, const float y, const float z)
{
	alListener3f(AL_POSITION, x, y, z);
}

void OpenALManager::setListenerVelocity(const float x, const float y, const float z)
{
	alListener3f(AL_VELOCITY, x, y, z);
}

void OpenALManager::setListenerOrientation(const float x, const float y, const float z)
{
	alListener3f(AL_ORIENTATION, x, y, z);
}

int OpenALManager::loadFile(const string &filePath)
{ return 0;
	// Load wav file // TODO: Make sure the file type is supported
	if(filePath.empty()) return -1;
	string strCopy(filePath);
	return loadSound(strCopy); // TODO: AssetLoader::loadSound();
}

void OpenALManager::deleteBuffer(const int bufferId)
{
	if(bufferId < 0) return;
	uint ubufferId = bufferId;
	alDeleteBuffers(1, &ubufferId);
}
#endif NO