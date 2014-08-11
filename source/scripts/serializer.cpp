#include "serializer.h"
#include <x2d/engine.h>
#include <x2d/console.h>
#include <x2d/filesystem.h>
#include <x2d/scriptengine.h>

AS_REG_REF(Serializer)

//asIScriptEngine *scriptEngine = 0;

int Serializer::Register(asIScriptEngine *scriptEngine)
{
	//::scriptEngine = scriptEngine;

	int r = 0;

	r = scriptEngine->RegisterObjectMethod("Serializer", "void store(?&in)", asMETHODPR(Serializer, store, (void*,int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "void store(int64&in)", asMETHODPR(Serializer, store, (asINT64&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "void store(double&in)", asMETHODPR(Serializer, store, (double&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "void store(string&in)", asMETHODPR(Serializer, store, (string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "bool restore(?&out)", asMETHODPR(Serializer, restore, (void*,int), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "bool restore(int64&out)", asMETHODPR(Serializer, restore, (asINT64&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "bool restore(double&out)", asMETHODPR(Serializer, restore, (double&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Serializer", "bool restore(string&out)", asMETHODPR(Serializer, restore, (string&), void), asCALL_THISCALL); AS_ASSERT

	return r;
}

Serializer::Serializer(stringstream &ss) :
	m_data(ss)
{
}

void Serializer::store(void *value, int typeId)
{
	// Call serialize on the given object
	if(value && (typeId & asTYPEID_MASK_OBJECT))
	{
		// Get object pointer
		void *objectPtr = value;
		if(typeId & asTYPEID_OBJHANDLE) {
			objectPtr = *(void**)value; // Dereference handles
		}

		// Serialize script object
		asIScriptObject *object = (asIScriptObject*)objectPtr;
//		g_engine->getScriptEngine()->serialize(object, m_data);
	}
}

void Serializer::store(double &value)
{
	m_data << value << endl;
}

void Serializer::store(asINT64 &value)
{
	m_data << value << endl;
}

void Serializer::store(string &value)
{
	m_data << value << endl;
}

void Serializer::restore(void *value, int typeId)
{
	// Call deserialize on the given object
	if(value && (typeId & asTYPEID_MASK_OBJECT))
	{
		// Get object pointer
		void *objectPtr = value;
		if(typeId & asTYPEID_OBJHANDLE) {
			objectPtr = *(void**)value; // Dereference handles
		}

		// Get script object and 'serialize' funciton
		asIScriptObject *object = (asIScriptObject*)objectPtr;
//		*(void**)value = g_engine->getScriptEngine()->deserialize(m_data);
	}
}

void Serializer::restore(double &value)
{
	m_data >> value;
	m_data.ignore();
}

void Serializer::restore(asINT64 &value)
{
	m_data >> value;
	m_data.ignore();
}

void Serializer::restore(string &value)
{
	getline(m_data, value);
}