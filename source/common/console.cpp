//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "console.h"
#include "filesystem.h"
#include "debugger.h"

AS_REG_SINGLETON(XConsole)

int XConsole::Register(asIScriptEngine *scriptEngine)
{
	int r;

	r = scriptEngine->RegisterObjectMethod("XConsole", "void log(const string &in)", asMETHODPR(XConsole, log, (const string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XConsole", "string getLog(const string &in) const", asMETHOD(XConsole, getLog), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XConsole", "void clear()", asMETHOD(XConsole, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XConsole", "void export()", asMETHOD(XConsole, exportFile), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XConsole", "string readBuffer()", asMETHOD(XConsole, readBuffer), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XConsole", "bool hasBuffer() const", asMETHOD(XConsole, hasBuffer), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XConsole", "void clearBuffer()", asMETHOD(XConsole, clearBuffer), asCALL_THISCALL); AS_ASSERT

	return r;
}

XConsole::XConsole() :
	m_fileSystem(0), // Set by the engine
	m_debugger(0), // Set by the engine
	m_output(0),
	m_initialized(false) // Set by the engine
{
}

XConsole::~XConsole()
{
	delete m_output;
}

void XConsole::log(const string &msg)
{
	logf(msg.c_str());
}

void XConsole::call_log(const char *msg, va_list args)
{
	// Get string length
	int size = _vscprintf(msg, args) + 1;

	// Create out string
	string out;
	out.resize(size);
	
	// Parse varargs
#ifdef USE_CTR_SECURE
	vsprintf_s(&out[0], size, msg, args);
#else
	vsprintf(out, msg, args);
#endif

	// System log
	syslog(out);
	
	// Append message to log file
	if(XEngine::IsEnabled(X2D_EXPORT_LOG))
	{
		m_output->append(out);
		m_output->append("\n");
		m_output->flush();
	}

	// Send message to debugger
	if(m_debugger)
	{
		m_debugger->sendPacket(X2D_MESSAGE_PACKET, out.data());
	}

	// Append to console buffer
	m_log.append(out);
	m_buffer.append(out);
}

#define CALL_LOG(format, ...)						\
	int size = _scprintf(format, __VA_ARGS__) + 1;	\
	char *newMsg = new char[size];					\
	sprintf(newMsg, format, __VA_ARGS__);			\
	call_log(newMsg, args);							\
	delete newMsg;

void XConsole::logf(const char *msg, ...)
{
	// Get argument list
	va_list args;
	va_start(args, msg);

	asIScriptContext *ctx = m_initialized ? asGetActiveContext() : 0;
	if(ctx)
	{
		const char *objName = ctx->GetFunction()->GetObjectName();
		if(objName)
		{
			CALL_LOG("%s::%s(): %s", objName, ctx->GetFunction()->GetName(), msg);
		}else
		{
			CALL_LOG("%s(): %s", ctx->GetFunction()->GetName(), msg);
		}
	}else
	{
		CALL_LOG("%s", msg);
	}

	va_end(args);
}

string XConsole::getLog() const
{
	return m_log;
}

void XConsole::clear()
{
	m_log.clear();
}

void XConsole::exportFile() const
{
	XFileSystem::WriteFile(":/console.log", m_log);
}

string XConsole::readBuffer()
{
	string buffer = m_buffer;
	m_buffer.clear();
	return buffer;
}

bool XConsole::hasBuffer() const
{
	return m_buffer.size() > 0;
}

void XConsole::clearBuffer()
{
	m_buffer.clear();
}