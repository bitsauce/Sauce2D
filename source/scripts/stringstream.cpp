#include "stringstream.h"
#include "scriptarray.h"
#include "scriptgrid.h"

#include <x2d/engine.h>
#include <x2d/scriptengine.h>

AS_REG_REF(StringStream)	

static void SerializeString(StringStream &ss, const string &str)
{
	(stringstream&)(stringstream&)ss << str << endl;
}

static void DeserializeString(StringStream &ss, string &str)
{
	getline((stringstream&)ss, str);
}

int StringStream::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("StringStream", asBEHAVE_FACTORY,  "StringStream @f()", asFUNCTION(Factory), asCALL_CDECL); assert( r >= 0 );

	r = scriptEngine->RegisterObjectMethod("StringStream", "void write(?&in)", asMETHOD(StringStream, write), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("StringStream", "void read(?&out)", asMETHOD(StringStream, read), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("StringStream", "string str() const", asMETHODPR(StringStream, str, () const, string), asCALL_THISCALL); assert( r >= 0 );

	r = scriptEngine->RegisterObjectMethod("string", "void serialize(StringStream&) const", asFUNCTION(SerializeString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("string", "void deserialize(StringStream&)", asFUNCTION(DeserializeString), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	
	r = scriptEngine->RegisterObjectMethod("array<T>", "void serialize(StringStream&) const", asMETHOD(CScriptArray, serialize), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("array<T>", "void deserialize(StringStream&)", asMETHOD(CScriptArray, deserialize), asCALL_THISCALL); assert( r >= 0 );
	
	r = scriptEngine->RegisterObjectMethod("grid<T>", "void serialize(StringStream&) const", asMETHOD(CScriptGrid, serialize), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("grid<T>", "void deserialize(StringStream&)", asMETHOD(CScriptGrid, deserialize), asCALL_THISCALL); assert( r >= 0 );

	return r;
}

void StringStream::write(void *value, int typeId)
{
	g_engine->getScriptEngine()->serialize(value, typeId, *this);
}

void StringStream::read(void *value, int typeId)
{
	g_engine->getScriptEngine()->deserialize(value, typeId, *this);
}

string StringStream::str() const
{
	return m_stream.str();
}