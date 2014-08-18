#ifndef SCRIPTS_STRINGSTREAM_H
#define SCRIPTS_STRINGSTREAM_H

#include <x2d/base.h>
#include <x2d/util.h>

class StringStream
{
	AS_DECL_REF
public:
	StringStream() :
		m_stream()
	{
	}
	
	StringStream(const string &content) :
		m_stream(content)
	{
	}

	operator stringstream&()
	{
		return m_stream;
	}

	void write(void *value, int typeId);
	void read(void *value, int typeId);
	string str() const;

private:
	static StringStream *Factory()
	{
		return new StringStream();
	}

	stringstream m_stream;
};

#endif // SCRIPTS_STRINGSTREAM_H