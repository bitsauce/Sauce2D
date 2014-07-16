#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

class XDAPI AudioBuffer
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

	AudioBuffer(void *data, uint size, uint frequency, Format format);
	~AudioBuffer();

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

#endif // AUDIO_BUFFER_H