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

template<typename T> T &Get()
{
}

void CallFunction(XFuncCall &funccall)
{
	// TODO: REFACTORING
	//Get<xdIThreadManager>().setupThread();
	funccall.execute();
	//xdIThreadManager::Get().cleanupThread();
	asThreadCleanup();
}

void ConstructThreadDefault(thread *self)
{
	new (self) thread();
}

void ConstructThreadFuncCall(XFuncCall &funccall, thread *self)
{
	new (self) thread(CallFunction, funccall);
}

void DestructThread(thread *self)
{
	if(self->joinable())
		self->join();
	self->~thread();
}

void ConstructMutex(mutex *self)
{
	new (self) mutex();
}

void DestructMutex(mutex *self)
{
	self->unlock();
	((_Mutex_base*)self)->~_Mutex_base();
}

int RegisterScriptThread(asIScriptEngine *scriptEngine)
{
	int r = 0;

	r = scriptEngine->RegisterObjectType("thread", sizeof(thread), asOBJ_VALUE | asOBJ_APP_CLASS_CD/*A*/); assert( r >= 0 ); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("thread", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructThreadDefault), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("thread", asBEHAVE_CONSTRUCT, "void f(funccall &in)", asFUNCTION(ConstructThreadFuncCall), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("thread", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructThread), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("thread", "bool joinable() const", asMETHODPR(thread, joinable, () const, bool), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("thread", "void detach()", asMETHODPR(thread, detach, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("thread", "void join()", asMETHODPR(thread, join, (), void), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectType("mutex", sizeof(mutex), asOBJ_VALUE | asOBJ_APP_CLASS_CD/*A*/); assert( r >= 0 ); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("mutex", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructMutex), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("mutex", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructMutex), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("mutex", "void lock()", asMETHODPR(mutex, lock, (), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("mutex", "bool tryLock()", asMETHODPR(mutex, try_lock, (), bool), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("mutex", "void unlock()", asMETHODPR(mutex, unlock, (), void), asCALL_THISCALL); AS_ASSERT

	return r;
}