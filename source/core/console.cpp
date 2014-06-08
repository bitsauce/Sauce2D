//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/console.h>
#include <x2d/filesystem.h>
#include <x2d/debug.h>

AS_REG_SINGLETON(xdConsole, "ScriptConsole")

int xdConsole::Register(asIScriptEngine *scriptEngine)
{
	int r;

	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "void log(const string &in)", asMETHODPR(xdConsole, log, (const string &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "string getLog(const string &in) const", asMETHOD(xdConsole, getLog), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "void clear()", asMETHOD(xdConsole, clear), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "void export()", asMETHOD(xdConsole, exportFile), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "string readBuffer()", asMETHOD(xdConsole, readBuffer), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "bool hasBuffer() const", asMETHOD(xdConsole, hasBuffer), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptConsole", "void clearBuffer()", asMETHOD(xdConsole, clearBuffer), asCALL_THISCALL); AS_ASSERT

	return r;
}

xdConsole::xdConsole() :
	m_fileSystem(0), // Set by the engine
	m_debugger(0) // Set by the engine
{
}

void xdConsole::log(const string &msg)
{
	// Send to debugger
	if(m_debugger) {
		m_debugger->sendPacket(XD_MESSAGE_PACKET, msg.data());
	}

	// Append to console
	m_log.append(msg);
	m_buffer.append(msg);
}

void xdConsole::log(const char* msg, ...)
{
	// Phrase varargs
	char out[512];
	va_list args;
	va_start(args, msg);
#ifdef USE_CTR_SECURE
		vsprintf_s(out, msg, args);
#else
		vsprintf(out, msg, args);
#endif
	va_end(args);

	// Send to an overload
	log(string(out));
}

string xdConsole::getLog() const
{
	return m_log;
}

void xdConsole::clear()
{
	m_log.clear();
}

void xdConsole::exportFile() const
{
	m_fileSystem->writeFile(":/console.txt", m_log);
}

string xdConsole::readBuffer()
{
	string buffer = m_buffer;
	m_buffer.clear();
	return buffer;
}

bool xdConsole::hasBuffer() const
{
	return m_buffer.size() > 0;
}

void xdConsole::clearBuffer()
{
	m_buffer.clear();
}