#ifndef X2D_AUDIO_BUFFER_H
#define X2D_AUDIO_BUFFER_H

#include "common/config.h"

class XDAPI XAudioBuffer
{
public:
	
	enum Format
	{
		STEREO16,
		STEREO8,
		MONO16,
		MONO8
	};

public:
	AS_DECL_VALUE

	XAudioBuffer(void *data, uint size, uint frequency, Format format);
	~XAudioBuffer();

	Format getFormat() const;
	
	void *getData() const;

	uint getSize() const;

	uint getFrequency() const;


private:

	Format m_format;

	void *m_data;

	uint m_size;

	uint m_frequency;
};

#endif // X2D_AUDIO_BUFFER_H