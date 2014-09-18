#ifndef X2D_THREAD_H
#define X2D_THREAD_H

#include "../engine.h"

/*********************************************************************
**	A managed mutex class											**
**********************************************************************/
class XMutex
{
	AS_DECL_REF
public:
	XMutex();

	void lock();
	bool tryLock();
	void unlock();

	bool isLocked() const;

private:
	static XMutex *Factory() { return new XMutex; }

	mutex m_this;
	bool m_locked;
	thread::id m_owner;
};

/*********************************************************************
**	A managed thread class											**
**********************************************************************/
class XThread
{
	AS_DECL_REF
public:
	XThread(XFuncCall *func);
	~XThread();

	void start();
	void stop();
	void dump() const;

	bool isJoinable() const;
	void detach();
	void join();

	XFuncCall *getFuncCall() const;
	void setContext(asIScriptContext *ctx);

private:
	static XThread *Factory(XFuncCall *func) { return new XThread(func); }

	thread *m_this;
	XFuncCall *m_funcCall;
	asIScriptContext *m_context;
};

#endif // X2D_THREAD_H