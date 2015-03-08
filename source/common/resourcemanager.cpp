//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>
#include <x2d/audio.h>

map<string, void*> xd::ResourceManager::s_resources;

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
//  -  Alut source code: static BufferData *loadWavFile (InputStream *stream)
//     http://www.openal.org/repos/openal/tags/freealut_1_1_0/alut/alut/src/alutLoader.c

/*XSound *XAssetManager::getSound(string filePath, const XSoundFormat)
{
	// For now we're loading all sounds as WAV files
	const unsigned int BUFFER_SIZE = 32768;     // 32 KB buffers
	long bytes;
	vector <char> data;
	XAudioBuffer::Format format;
	uint freq;

	// Local resources
	FILE *f = NULL;
	char *array = NULL;

	// Main process
	util::toAbsoluteFilePath(filePath);

	// Open for binary reading
	fopen_s(&f, filePath.c_str(), "rb");
	if(!f)
	{
		LOG("Could not read wav file '%s'", filePath);
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
		LOG("Wav magic failed '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(std::string(magic) != "RIFF")
	{
		LOG("Wrong wav file format. This file is not a .wav file (no RIFF magic) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// Skip 4 bytes (file size)
	fseek(f,4,SEEK_CUR);

	// Check file format
	if(fread(magic, 4, 1, f) != 1)
	{
		LOG("More wav magic failed '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(string(magic) != "WAVE")
	{
		LOG("Wrong wav file format. This file is not a .wav file (no WAVE magic) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// check 'fmt ' sub chunk (1)
	if(fread(magic,4,1,f) != 1)
	{
		LOG("Failed to read subchunk '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(string(magic) != "fmt ")
	{
		LOG("Wrong wav file format. This file is not a .wav file (no 'fmt' subchunk) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// read (1)'s size
	if(fread(buffer32, 4, 1, f) != 1)
	{
		LOG("Failed to read subchunk size '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	unsigned long subChunk1Size = readByte32(buffer32);
	if(!(subChunk1Size >= 16))
	{
		LOG("Wrong wav file format. This file is not a .wav file ('fmt ' chunk too small, truncated file?) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// check PCM audio format
	if(fread(buffer16,2,1,f) != 1)
	{
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	unsigned short audioFormat = readByte16(buffer16);
	if(audioFormat != 1)
	{
		LOG("Wrong wav file format. This file is not a .wav file (audio format is not PCM) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	// read number of channels
	if(fread(buffer16, 2, 1, f) != 1)
	{
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned short channels = readByte16(buffer16);

	// read frequency (sample rate)
	if(fread(buffer32, 4, 1, f) != 1)
	{
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	freq = readByte32(buffer32);

	// skip 6 bytes (Byte rate (4), Block align (2))
	fseek(f, 6, SEEK_CUR);

	// read bits per sample
	if(fread(buffer16, 2, 1, f) != 1)
	{
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	unsigned short bps = readByte16(buffer16);

	if(channels == 1)
		format = (bps == 8) ? XAudioBuffer::MONO8 : XAudioBuffer::MONO16;
	else
		format = (bps == 8) ? XAudioBuffer::STEREO8 : XAudioBuffer::STEREO16;

	// check 'data' sub chunk (2)
	if(fread(magic, 4, 1, f) != 1)
	{ 
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}
	if(std::string(magic) != "data")
	{
		LOG("Wrong wav file format. This file is not a .wav file (no data subchunk) '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	if(fread(buffer32,4,1,f) != 1)
	{
		LOG("Failed to read wav file '%s'", filePath);
		return WAV_LOAD_ERROR;
	}

	array = new char[BUFFER_SIZE];
	
	unsigned long subChunk2Size = readByte32(buffer32);
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
	}

	delete[] array;
	array = NULL;

	fclose(f);
	f = NULL;
	
	return new XAudioBuffer(data.data(), data.size(), freq, format);
}*/