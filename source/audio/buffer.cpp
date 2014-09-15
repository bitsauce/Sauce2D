#include "buffer.h"

AS_REG_VALUE(XAudioBuffer)

int XAudioBuffer::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	return r;
}

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