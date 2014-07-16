#include <x2d/audio.h>

AS_REG_VALUE(AudioBuffer)

int AudioBuffer::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	return r;
}

AudioBuffer::AudioBuffer(void *data, uint size, uint frequency, Format format) :
	m_frequency(frequency),
	m_format(format)
{
	m_data = new uchar[size];
	memcpy(m_data, data, size);
	m_size = size;
}

AudioBuffer::~AudioBuffer()
{
	delete m_data;
}

void *AudioBuffer::getData() const
{
	return m_data;
}

uint AudioBuffer::getSize() const
{
	return m_size;
}

uint AudioBuffer::getFrequency() const
{
	return m_frequency;
}

AudioBuffer::Format AudioBuffer::getFormat() const
{
	return m_format;
}