#include "stringstream.h"
#include "scriptarray.h"
#include "scriptgrid.h"

#include "common/engine.h"

AS_REG_REF(StringStream)

int StringStream::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("StringStream", asBEHAVE_FACTORY,  "StringStream @f()", asFUNCTION(Factory), asCALL_CDECL); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const bool)", asMETHODPR(StringStream, write, (const bool), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int8)", asMETHODPR(StringStream, write, (const char), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int16)", asMETHODPR(StringStream, write, (const short), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int)", asMETHODPR(StringStream, write, (const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int64)", asMETHODPR(StringStream, write, (const long), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const float)", asMETHODPR(StringStream, write, (const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const double)", asMETHODPR(StringStream, write, (const double), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const string&in)", asMETHODPR(StringStream, write, (const string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const Vector2&in)", asMETHODPR(StringStream, write, (const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(bool&out)", asMETHODPR(StringStream, read, (bool&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int8&out)", asMETHODPR(StringStream, read, (char&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int16&out)", asMETHODPR(StringStream, read, (short&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int&out)", asMETHODPR(StringStream, read, (int&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int64&out)", asMETHODPR(StringStream, read, (long&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(float&out)", asMETHODPR(StringStream, read, (float&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(double&out)", asMETHODPR(StringStream, read, (double&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(string&out)", asMETHODPR(StringStream, read, (string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(Vector2&out)", asMETHODPR(StringStream, read, (Vector2&), void), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("StringStream", "string str() const", asMETHODPR(StringStream, str, () const, string), asCALL_THISCALL); AS_ASSERT

	return r;
}

void StringStream::write(const bool value)
{
	m_stream << value << endl;
}

void StringStream::write(const char value)
{
	m_stream << value << endl;
}

void StringStream::write(const short value)
{
	m_stream << value << endl;
}

void StringStream::write(const int value)
{
	m_stream << value << endl;
}

void StringStream::write(const long value)
{
	m_stream << value << endl;
}

void StringStream::write(const float value)
{
	m_stream << value << endl;
}

void StringStream::write(const double value)
{
	m_stream << value << endl;
}

void StringStream::write(const string &value)
{
	m_stream << value << endl;
}

void StringStream::write(const Vector2 &value)
{
	write(value.x);
	write(value.y);
}

void StringStream::read(bool &b)
{
	m_stream >> b; m_stream.ignore();
}

void StringStream::read(char &c)
{
	m_stream >> c; m_stream.ignore();
}

void StringStream::read(short &s)
{
	m_stream >> s; m_stream.ignore();
}

void StringStream::read(int &i)
{
	m_stream >> i; m_stream.ignore();
}

void StringStream::read(long &value)
{
	m_stream >> value; m_stream.ignore();
}

void StringStream::read(float &value)
{
	m_stream >> value; m_stream.ignore();
}

void StringStream::read(double &d)
{
	m_stream >> d; m_stream.ignore();
}

void StringStream::read(string &str)
{
	getline(m_stream, str);
}

void StringStream::read(Vector2 &value)
{
	read(value.x);
	read(value.y);
}

string StringStream::str() const
{
	return m_stream.str();
}