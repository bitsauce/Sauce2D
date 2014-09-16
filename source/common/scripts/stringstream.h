#ifndef SCRIPTS_STRINGSTREAM_H
#define SCRIPTS_STRINGSTREAM_H

#include "common/engine.h"

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
	
	void write(const bool);
	void write(const char);
	void write(const short);
	void write(const int);
	void write(const long);
	void write(const float);
	void write(const double);
	void write(const string&);
	void write(const Vector2&);
	
	void read(bool&);
	void read(char&);
	void read(short&);
	void read(int&);
	void read(long&);
	void read(float&);
	void read(double&);
	void read(string&);
	void read(Vector2&);

	string str() const;

private:
	static StringStream *Factory()
	{
		return new StringStream();
	}

	stringstream m_stream;
};

#endif // SCRIPTS_STRINGSTREAM_H