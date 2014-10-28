//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>
#include <x2d/extention.h>

/*********************************************************************
**	A managed mutex class											**
**********************************************************************/

AS_REG_REF(XMutex, "Mutex")

int XMutex::Register(asIScriptEngine *scriptEngine)
{
	int r;
	r = scriptEngine->RegisterObjectBehaviour("Mutex", asBEHAVE_FACTORY, "Mutex @f()", asFUNCTION(XMutex::Factory), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Mutex", "void lock()", asMETHOD(XMutex, lock), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Mutex", "bool tryLock()", asMETHOD(XMutex, tryLock), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Mutex", "void unlock()", asMETHOD(XMutex, unlock), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Mutex", "bool isLocked() const", asMETHOD(XMutex, isLocked), asCALL_THISCALL); AS_ASSERT
	return r;
}

XMutex::XMutex() :
	m_this(),
	m_locked(false)
{
}

void XMutex::lock()
{
	m_this.lock();
	m_locked = true;
	m_owner = this_thread::get_id();
}

bool XMutex::tryLock()
{
	bool result = m_this.try_lock();
	if(result)
	{
		m_locked = true;
		m_owner = this_thread::get_id();
	}
	return result;
}

void XMutex::unlock()
{
	if(m_owner == this_thread::get_id())
	{
		m_this.unlock();
		m_locked = false;
	}
	else
	{
		LOG("XMutex::unlock() - Has to be called on the same thread which locked it");
	}
}

bool XMutex::isLocked() const
{
	return m_locked;
}

/*********************************************************************
**	A managed thread class											**
**********************************************************************/

AS_REG_REF(XThread, "Thread")

int XThread::Register(asIScriptEngine *scriptEngine)
{
	int r;
	r = scriptEngine->RegisterObjectBehaviour("Thread", asBEHAVE_FACTORY, "Thread @f(FuncCall@)", asFUNCTIONPR(XThread::Factory, (XFuncCall*), XThread*), asCALL_CDECL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Thread", "void start()", asMETHOD(XThread, start), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Thread", "void stop()", asMETHOD(XThread, stop), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Thread", "void dump() const", asMETHOD(XThread, dump), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Thread", "bool isJoinable() const", asMETHOD(XThread, isJoinable), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Thread", "void detach()", asMETHOD(XThread, detach), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Thread", "void join()", asMETHOD(XThread, join), asCALL_THISCALL); AS_ASSERT
	return r;
}

XThread::XThread(XFuncCall *funcCall) :
	m_this(0),
	m_funcCall(funcCall),
	m_context(0)
{
	if(m_funcCall)
	{
		m_funcCall->addRef();
	}
}

XThread::~XThread()
{
	if(m_funcCall)
	{
		m_funcCall->release();
	}

	if(m_this)
	{
		if(m_this->joinable())
			m_this->detach();
		delete m_this;
	}
}

bool XThread::isJoinable() const
{
	return m_this ? m_this->joinable() : false;
}

void XThread::detach()
{
	if(m_this && m_this->joinable())
	{
		m_this->detach();
	}
}

void XThread::join()
{
	if(m_this && m_this->joinable())
	{
		m_this->join();
	}
}

XFuncCall *XThread::getFuncCall() const
{
	return m_funcCall;
}

void XThread::setContext(asIScriptContext *ctx)
{
	m_context = ctx;
}

void CallFunction(XThread *thread)
{
	// Create and set context
	asIScriptContext *ctx = XScriptEngine::CreateContext();
	thread->setContext(ctx);
	thread->getFuncCall()->execute(ctx);
	thread->setContext(0);

	asThreadCleanup();
}

void XThread::start()
{
	if(!m_this && m_funcCall)
	{
		m_this = new thread(CallFunction, this);
	}
	else
	{
		LOG("XThread::start() - Already running");
	}
}

void XThread::stop()
{
	if(m_context)
	{
		m_context->Abort();
	}
}

void XThread::dump() const
{
	if(m_context)
	{
		const char *file;
		int line = m_context->GetLineNumber(0, 0, &file);
		asIScriptFunction *func = m_context->GetFunction();
		LOG("CURRENT FUNCTION: %s", func ? func->GetName() : "NONE");
		LOG("LOCATION: %s (%i)", file ? file : "NONE", line);
	}
}