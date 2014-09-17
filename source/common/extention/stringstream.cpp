//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>

AS_REG_REF(XStringStream, "StringStream")

int XStringStream::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("StringStream", asBEHAVE_FACTORY,  "StringStream @f()", asFUNCTION(Factory), asCALL_CDECL); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const bool)", asMETHODPR(XStringStream, write, (const bool), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int8)", asMETHODPR(XStringStream, write, (const char), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int16)", asMETHODPR(XStringStream, write, (const short), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int)", asMETHODPR(XStringStream, write, (const int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const int64)", asMETHODPR(XStringStream, write, (const long), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const float)", asMETHODPR(XStringStream, write, (const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const double)", asMETHODPR(XStringStream, write, (const double), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const string&in)", asMETHODPR(XStringStream, write, (const string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(const Vector2&in)", asMETHODPR(XStringStream, write, (const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(bool&out)", asMETHODPR(XStringStream, read, (bool&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int8&out)", asMETHODPR(XStringStream, read, (char&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int16&out)", asMETHODPR(XStringStream, read, (short&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int&out)", asMETHODPR(XStringStream, read, (int&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(int64&out)", asMETHODPR(XStringStream, read, (long&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(float&out)", asMETHODPR(XStringStream, read, (float&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(double&out)", asMETHODPR(XStringStream, read, (double&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(string&out)", asMETHODPR(XStringStream, read, (string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(Vector2&out)", asMETHODPR(XStringStream, read, (Vector2&), void), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("StringStream", "string str() const", asMETHODPR(XStringStream, str, () const, string), asCALL_THISCALL); AS_ASSERT

	return r;
}

void XStringStream::write(const bool value)
{
	m_stream << value << endl;
}

void XStringStream::write(const char value)
{
	m_stream << value << endl;
}

void XStringStream::write(const short value)
{
	m_stream << value << endl;
}

void XStringStream::write(const int value)
{
	m_stream << value << endl;
}

void XStringStream::write(const long value)
{
	m_stream << value << endl;
}

void XStringStream::write(const float value)
{
	m_stream << value << endl;
}

void XStringStream::write(const double value)
{
	m_stream << value << endl;
}

void XStringStream::write(const string &value)
{
	m_stream << value << endl;
}

void XStringStream::write(const Vector2 &value)
{
	write(value.x);
	write(value.y);
}

void XStringStream::read(bool &b)
{
	m_stream >> b; m_stream.ignore();
}

void XStringStream::read(char &c)
{
	m_stream >> c; m_stream.ignore();
}

void XStringStream::read(short &s)
{
	m_stream >> s; m_stream.ignore();
}

void XStringStream::read(int &i)
{
	m_stream >> i; m_stream.ignore();
}

void XStringStream::read(long &value)
{
	m_stream >> value; m_stream.ignore();
}

void XStringStream::read(float &value)
{
	m_stream >> value; m_stream.ignore();
}

void XStringStream::read(double &d)
{
	m_stream >> d; m_stream.ignore();
}

void XStringStream::read(string &str)
{
	getline(m_stream, str);
}

void XStringStream::read(Vector2 &value)
{
	read(value.x);
	read(value.y);
}

string XStringStream::str() const
{
	return m_stream.str();
}