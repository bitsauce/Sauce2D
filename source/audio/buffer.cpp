//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/audio.h>

XAudioBuffer::XAudioBuffer(void *data, uint size, uint frequency, Format format) :
	m_frequency(frequency),
	m_format(format)
{
	m_data = new uchar[size];
	memcpy(m_data, data, size);
	m_size = size;
}

XAudioBuffer::~XAudioBuffer()
{
	delete m_data;
}

void *XAudioBuffer::getData() const
{
	return m_data;
}

uint XAudioBuffer::getSize() const
{
	return m_size;
}

uint XAudioBuffer::getFrequency() const
{
	return m_frequency;
}

XAudioBuffer::Format XAudioBuffer::getFormat() const
{
	return m_format;
}